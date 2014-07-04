// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#ifndef sw_x0_Base64_h
#define sw_x0_Base64_h (1)

#include <x0/Api.h>
#include <x0/Buffer.h>

namespace x0 {

class X0_API Base64 {
 private:
  static const char base64_[];
  static const unsigned char pr2six_[256];

 public:
  static int encodeLength(int sourceLength);

  static std::string encode(const std::string& text);
  static std::string encode(const Buffer& buffer);
  static std::string encode(const unsigned char* buffer, int length);

  static int decodeLength(const std::string& buffer);
  static int decodeLength(const char* buffer);

  static Buffer decode(const std::string& base64Value);
  static int decode(const char* input, unsigned char* output);
  static Buffer decode(const BufferRef& base64Value);
};

}  // namespace x0

#endif
