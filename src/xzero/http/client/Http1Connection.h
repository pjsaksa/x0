// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2018 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/Buffer.h>
#include <xzero/CompletionHandler.h>
#include <xzero/net/TcpConnection.h>
#include <xzero/net/EndPointWriter.h>
#include <xzero/http/http1/Generator.h>
#include <xzero/http/http1/Parser.h>
#include <xzero/http/client/HttpTransport.h>
#include <xzero/http/HttpListener.h>

namespace xzero {
namespace http {
namespace client {

/**
 * HTTP/1 client-side transport protocol implementation.
 */
class Http1Connection
    : public TcpConnection,
      public HttpTransport,
      private HttpListener {
public:
  /**
   * Initializes the client-side HTTP/1 transport layer.
   *
   * @param channel HTTP channel to report HTTP and error events to.
   * @param endpoint communication TcpEndPoint
   * @param executor connection-level executor API
   */
  Http1Connection(HttpListener* channel, TcpEndPoint* endpoint, Executor* executor);
  ~Http1Connection();

  // HttpTransport overrides
  void setListener(HttpListener* channel) override;
  void send(const HttpRequestInfo& requestInfo,
            CompletionHandler onComplete) override;
  void send(const HttpRequestInfo& requestInfo,
            const BufferRef& chunk,
            CompletionHandler onComplete) override;
  void send(const HttpRequestInfo& requestInfo,
            Buffer&& chunk,
            CompletionHandler onComplete) override;
  void send(const HttpRequestInfo& requestInfo,
            FileView&& chunk,
            CompletionHandler onComplete) override;
  void send(const HttpRequestInfo& requestInfo,
            HugeBuffer&& chunk,
            CompletionHandler onComplete) override;
  void send(const BufferRef& chunk, CompletionHandler onComplete) override;
  void send(Buffer&& chunk, CompletionHandler onComplete) override;
  void send(FileView&& chunk, CompletionHandler onComplete) override;
  void send(HugeBuffer&& chunk, CompletionHandler onComplete) override;
  void completed() override;
  void abort() override;

  // TcpConnection overrides
  void onReadable() override;
  void onWriteable() override;
  void onInterestFailure(const std::exception& error) override;

  // HttpListener overrides
  void onMessageBegin(HttpVersion version, HttpStatus code,
                      const BufferRef& text) override;
  void onMessageHeader(const BufferRef& name, const BufferRef& value) override;
  void onMessageHeaderEnd() override;
  void onMessageContent(const BufferRef& chunk) override;
  void onMessageContent(FileView&& chunk) override;
  void onMessageEnd() override;
  void onError(std::error_code ec) override;

 private:
  void onRequestComplete(bool success);
  void onResponseComplete(bool success);
  void parseFragment();

  void setCompleter(CompletionHandler cb);
  void notifySuccess() { invokeCompleter(true); }
  void notifyFailure() { invokeCompleter(false); }
  void invokeCompleter(bool success);

 private:
  HttpListener* channel_;
  CompletionHandler onComplete_;

  // request generator
  EndPointWriter writer_;
  http1::Generator generator_;

  // response parser
  http1::Parser parser_;
  Buffer inputBuffer_;
  size_t inputOffset_;

  bool expectsBody_;
  bool responseComplete_;
  size_t keepAliveCount_;
};

} // namespace client
} // namespace http
} // namespace xzero
