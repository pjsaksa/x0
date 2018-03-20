// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2017 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero-flow/NativeCallback.h>
#include <xzero/net/IPAddress.h>
#include <xzero/net/Cidr.h>
#include <xzero/RegExp.h>

namespace xzero::flow {

// constructs a handler callback
NativeCallback::NativeCallback(Runtime* runtime, const std::string& _name)
    : runtime_(runtime),
      isHandler_(true),
      verifier_(),
      function_(),
      signature_(),
      attributes_(0) {
  signature_.setName(_name);
  signature_.setReturnType(FlowType::Boolean);
}

// constructs a function callback
NativeCallback::NativeCallback(Runtime* runtime, const std::string& _name,
                               FlowType _returnType)
    : runtime_(runtime),
      isHandler_(false),
      verifier_(),
      function_(),
      signature_(),
      attributes_(0) {
  signature_.setName(_name);
  signature_.setReturnType(_returnType);
}

NativeCallback::~NativeCallback() {
  for (size_t i = 0, e = defaults_.size(); i != e; ++i) {
    FlowType type = signature_.args()[i];
    switch (type) {
      case FlowType::Boolean:
        delete (bool*)defaults_[i];
        break;
      case FlowType::Number:
        delete (FlowNumber*)defaults_[i];
        break;
      case FlowType::String:
        delete (FlowString*)defaults_[i];
        break;
      case FlowType::IPAddress:
        delete (IPAddress*)defaults_[i];
        break;
      case FlowType::Cidr:
        delete (Cidr*)defaults_[i];
        break;
      case FlowType::RegExp:
        delete (RegExp*)defaults_[i];
        break;
      case FlowType::Handler:
      case FlowType::StringArray:
      case FlowType::IntArray:
      default:
        break;
    }
  }
}

bool NativeCallback::isHandler() const noexcept {
  return isHandler_;
}

bool NativeCallback::isFunction() const noexcept {
  return !isHandler_;
}

const std::string NativeCallback::name() const noexcept {
  return signature_.name();
}

const Signature& NativeCallback::signature() const noexcept {
  return signature_;
}

int NativeCallback::findParamByName(const std::string& name) const {
  for (int i = 0, e = names_.size(); i != e; ++i)
    if (names_[i] == name)
      return i;

  return -1;
}

NativeCallback& NativeCallback::setNoReturn() noexcept {
  attributes_ |= (unsigned) Attribute::NoReturn;
  return *this;
}

NativeCallback& NativeCallback::setReadOnly() noexcept {
  attributes_ |= (unsigned) Attribute::SideEffectFree;
  return *this;
}

NativeCallback& NativeCallback::setExperimental() noexcept {
  attributes_ |= (unsigned) Attribute::Experimental;
  return *this;
}

void NativeCallback::invoke(Params& args) const {
  function_(args);
}

}  // namespace xzero::flow
