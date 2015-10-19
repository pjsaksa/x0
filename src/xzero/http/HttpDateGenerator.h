// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/http/Api.h>
#include <xzero/Buffer.h>
#include <xzero/DateTime.h>
#include <mutex>

namespace xzero {

class WallClock;

namespace http {

/**
 * API to generate an HTTP conform Date response header field value.
 */
class XZERO_BASE_HTTP_API HttpDateGenerator {
 public:
  explicit HttpDateGenerator(WallClock* clock);

  WallClock* clock() const { return clock_; }
  void setClock(WallClock* clock) { clock_ = clock; }

  void update();
  void fill(Buffer* target);

 private:
  WallClock* clock_;
  DateTime current_;
  Buffer buffer_;
  std::mutex mutex_;
};

} // namespace http
} // namespace xzero
