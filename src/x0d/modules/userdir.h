// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2018 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <x0d/Module.h>
#include <xzero/http/HttpOutputCompressor.h>
#include <string>

namespace x0d {

class UserdirModule : public Module {
 public:
  explicit UserdirModule(Daemon* d);
  ~UserdirModule();

 private:
  // setup properties
  void userdir_name(Params& args);
  std::error_code validate(std::string& path);

  // main functions
  void userdir(Context* cx, Params& args);

 private:
  std::string dirname_;
};

} // namespace x0d
