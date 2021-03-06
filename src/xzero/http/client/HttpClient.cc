// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2018 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/http/client/HttpClient.h>
#include <xzero/http/client/HttpTransport.h>
#include <xzero/http/client/Http1Connection.h>
#include <xzero/http/HttpRequest.h>
#include <xzero/http/HttpStatus.h>
#include <xzero/net/TcpEndPoint.h>
#include <xzero/net/SslEndPoint.h>
#include <xzero/net/DnsClient.h>
#include <xzero/net/InetAddress.h>
#include <xzero/net/IPAddress.h>
#include <xzero/io/FileView.h>
#include <xzero/RuntimeError.h>
#include <xzero/logging.h>
#include <algorithm>

namespace xzero::http::client {

// {{{ ResponseBuilder
class HttpClient::ResponseBuilder : public HttpListener {
 public:
  explicit ResponseBuilder(Promise<Response> promise);
  ~ResponseBuilder();

  void onMessageBegin(HttpVersion version, HttpStatus code, const BufferRef& text) override;
  void onMessageHeader(const BufferRef& name, const BufferRef& value) override;
  void onMessageHeaderEnd() override;
  void onMessageContent(const BufferRef& chunk) override;
  void onMessageContent(FileView&& chunk) override;
  void onMessageEnd() override;
  void onError(std::error_code ec) override;

 private:
  Promise<Response> promise_;
  Response response_;
};

HttpClient::ResponseBuilder::ResponseBuilder(Promise<Response> promise)
    : promise_(promise),
      response_() {
}

HttpClient::ResponseBuilder::~ResponseBuilder() {
}

void HttpClient::ResponseBuilder::onMessageBegin(HttpVersion version,
                                                 HttpStatus code,
                                                 const BufferRef& text) {
  response_.setVersion(version);
  response_.setStatus(code);
  response_.setReason(text.str());
}

void HttpClient::ResponseBuilder::onMessageHeader(const BufferRef& name,
                                                  const BufferRef& value) {
  response_.headers().push_back(name.str(), value.str());
}

void HttpClient::ResponseBuilder::onMessageHeaderEnd() {
}

void HttpClient::ResponseBuilder::onMessageContent(const BufferRef& chunk) {
  response_.content().write(chunk);
}

void HttpClient::ResponseBuilder::onMessageContent(FileView&& chunk) {
  response_.content().write(std::move(chunk));
}

void HttpClient::ResponseBuilder::onMessageEnd() {
  response_.setContentLength(response_.content().size());
  promise_.success(response_);
}

void HttpClient::ResponseBuilder::onError(std::error_code ec) {
  promise_.failure(ec);
}
// }}}

template<typename T> static bool isConnectionHeader(const T& name) { // {{{
  static const std::vector<T> connectionHeaderFields = {
    "Connection",
    "Content-Length",
    "Close",
    "Keep-Alive",
    "TE",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
  };

  for (const auto& test: connectionHeaderFields)
    if (iequals(name, test))
      return true;

  return false;
} // }}}

HttpClient::HttpClient(Executor* executor,
                       const InetAddress& upstream)
    : HttpClient{executor,
                 upstream,
                 10_seconds,   // connectTimeout
                 5_minutes,    // readTimeout
                 10_seconds,   // writeTimeout
                 60_seconds} { // keepAlive
}

HttpClient::HttpClient(Executor* executor,
                       const InetAddress& upstream,
                       Duration connectTimeout,
                       Duration readTimeout,
                       Duration writeTimeout,
                       Duration keepAlive)
    : executor_{executor},
      address_{upstream},
      connectTimeout_{connectTimeout},
      readTimeout_{readTimeout},
      writeTimeout_{writeTimeout},
      keepAlive_{keepAlive},
      contexts_{} {
}

HttpClient::HttpClient(HttpClient&& other)
    : executor_{std::move(other.executor_)},
      address_{std::move(other.address_)},
      connectTimeout_{std::move(other.connectTimeout_)},
      readTimeout_{std::move(other.readTimeout_)},
      writeTimeout_{std::move(other.writeTimeout_)},
      keepAlive_{std::move(other.keepAlive_)},
      contexts_{} {
}

static std::string extractServerNameFromHostHeader(const std::string& hostHeader) {
  size_t i = hostHeader.find(':');
  if (i != std::string::npos) {
    return hostHeader.substr(0, i);
  } else {
    return hostHeader;
  }
}

Future<HttpClient::Response> HttpClient::send(const std::string& method,
                                              const Uri& url,
                                              const HeaderFieldList& headers) {
  return send(Request{HttpVersion::VERSION_1_1,
                      method,
                      url.toString(),
                      headers,
                      false /* secure */,
                      HugeBuffer()});
}

Future<HttpClient::Response> HttpClient::send(const Request& request) {
  Promise<Response> promise;

  std::unique_ptr<Context> cx = std::make_unique<Context>(
      executor_, readTimeout_, writeTimeout_,
      std::bind(&HttpClient::releaseContext, this, std::placeholders::_1),
      request, std::make_unique<ResponseBuilder>(promise));

  contexts_.emplace_back(std::move(cx));
  contexts_.back()->execute(address_, connectTimeout_);

  return promise.future();
}

void HttpClient::send(const Request& request, HttpListener* responseListener) {
  std::unique_ptr<Context> cx = std::make_unique<Context>(
      executor_, readTimeout_, writeTimeout_,
      std::bind(&HttpClient::releaseContext, this, std::placeholders::_1),
      request, responseListener);

  contexts_.emplace_back(std::move(cx));
  contexts_.back()->execute(address_, connectTimeout_);
}

void HttpClient::releaseContext(Context* ctx) {
  auto i = std::find_if(contexts_.begin(), contexts_.end(), [&](const auto& x) {
      return x.get() == ctx; });

  if (i != contexts_.end()) {
    contexts_.erase(i);
  }
}

// --------------------------------------------------------------------------

HttpClient::Context::Context(Executor* executor,
                             Duration readTimeout,
                             Duration writeTimeout,
                             std::function<void(Context*)> done,
                             const Request& req,
                             HttpListener* resp)
  : executor_{executor},
    endpoint_(readTimeout, writeTimeout, executor, nullptr),
    done_{done},
    request_{req},
    listener_{resp},
    ownedListener_{} {
}

HttpClient::Context::Context(Executor* executor,
                             Duration readTimeout,
                             Duration writeTimeout,
                             std::function<void(Context*)> done,
                             const Request& req,
                             std::unique_ptr<HttpListener> resp)
  : executor_{executor},
    endpoint_(readTimeout, writeTimeout, executor, nullptr),
    done_{done},
    request_{req},
    listener_{resp.get()},
    ownedListener_{std::move(resp)} {
}

void HttpClient::Context::execute(const InetAddress& address, Duration connectTimeout) {
  endpoint_.connect(
      address,
      connectTimeout,
      [this]() { onConnected(); },
      [this](std::error_code ec) { listener_->onError(ec); done_(this); });
}

void HttpClient::Context::onConnected() {
  endpoint_.setConnection(std::make_unique<Http1Connection>(listener_, &endpoint_, executor_));

  // dynamic_cast, as we're having most-likely multiple inheritance here
  HttpTransport* channel = dynamic_cast<HttpTransport*>(endpoint_.connection());
  assert(channel != nullptr);

  channel->setListener(listener_);
  channel->send(request_, nullptr);
  channel->send(request_.getContent().getBuffer(), nullptr);
  channel->completed();
}

} // namespace xzero::http::client
