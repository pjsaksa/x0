// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <gtest/gtest.h>
#include <x0/Buffer.h>
#include <x0/Cidr.h>
#include <cstdio>

using namespace x0;

TEST(Cidr, contains) {
  Cidr cidr(IPAddress("192.168.0.0"), 24);
  IPAddress ip0("192.168.0.1");
  IPAddress ip1("192.168.1.1");

  ASSERT_TRUE(cidr.contains(ip0));
  ASSERT_FALSE(cidr.contains(ip1));
}

TEST(Cidr, equals) {
  ASSERT_EQ(Cidr(IPAddress("0.0.0.0"), 0), Cidr(IPAddress("0.0.0.0"), 0));
  ASSERT_EQ(Cidr(IPAddress("1.2.3.4"), 24), Cidr(IPAddress("1.2.3.4"), 24));

  ASSERT_NE(Cidr(IPAddress("1.2.3.4"), 24), Cidr(IPAddress("1.2.1.4"), 24));
  ASSERT_NE(Cidr(IPAddress("1.2.3.4"), 24), Cidr(IPAddress("1.2.3.4"), 23));
}
