// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2017 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/io/SystemPipe.h>
#include <xzero/defines.h>
#include <xzero/sysconfig.h>
#include <xzero/RuntimeError.h>
#include <xzero/logging.h>

#if defined(XZERO_OS_UNIX)
#include <fcntl.h>
#include <unistd.h>
#endif

#if defined(XZERO_OS_WINDOWS)
#include <WinSock2.h>
#include <io.h>
#endif

namespace xzero {

#if defined(XZERO_OS_WIN32)
struct WinSocket {
  SOCKET handle;

  WinSocket() : WinSocket{ INVALID_SOCKET } {}
  WinSocket(SOCKET h) : handle{h} {}

  WinSocket(const WinSocket&) = delete;
  WinSocket& operator=(const WinSocket&) = delete;

  ~WinSocket () {
    if (handle != INVALID_SOCKET) {
      closesocket(handle);
    }
  }

  WinSocket& operator=(SOCKET other) {
    if (handle != INVALID_SOCKET) {
      closesocket(handle);
    }
    handle = other;
    return *this;
  }

  bool operator!() const noexcept {
    return handle == INVALID_SOCKET;
  }

  operator SOCKET () {
    return handle;
  }

  int make_filedes() {
    int fd = _open_osfhandle(handle, 0);
    handle = INVALID_SOCKET;
    return fd;
  }
};

int pipe(int fd[2]) {
  // XXX setup listener
  WinSocket listener = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
  sockaddr_in addr;
  ZeroMemory(&addr, sizeof(addr));
  int addr_size = sizeof(addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
  addr.sin_port = 0;

  if (bind(listener, (struct sockaddr *)&addr, addr_size) == -1)
    return -1;

  if (getsockname(listener, (sockaddr *)&addr, &addr_size) == -1)
    return -1;

  if (listen(listener, 1) == -1)
    return -1;

  // XXX setup sender/receiver
  WinSocket receiver = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, 0);
  if (connect(receiver, (struct sockaddr *)&addr, addr_size) == -1)
    return -1;

  WinSocket sender = ::accept(listener.handle(), 0, 0);
  if (!sender)
    return -1;

  sockaddr_in addr2;
  int addr2_size = sizeof(addr2);
  getpeername(receiver, (sockaddr*)&addr2, &addr2_size);
  getpeername(sender, (sockaddr*)&addr, &addr_size);

  fd[0] = receiver.make_filedes();
  fd[1] = sender.make_filedes();
  return 0;
}
#endif

SystemPipe::SystemPipe() {
  if (pipe(fds_) < 0) {
    RAISE_ERRNO(errno);
  }
}

SystemPipe::~SystemPipe() {
  closeReader();
  closeWriter();
}

void SystemPipe::setNonBlocking(bool enable) {
#if defined(F_SETFL) && defined(O_NONBLOCK)
  if (enable) {
    fcntl(fds_[0], F_SETFL, O_NONBLOCK);
    fcntl(fds_[1], F_SETFL, O_NONBLOCK);
  } else {
    for (int i = 0; i < 2; ++i) {
      int flags = fcntl(fds_[i], F_GETFL);
      if (flags != -1) {
        fcntl(fds_[i], F_SETFL, flags & ~O_NONBLOCK);
      }
    }
  }
#else
  // TODO: omg, world's going to blow up (Windows)
#endif
}

void SystemPipe::closeReader() {
  if (fds_[0] != -1) {
    ::close(fds_[0]);
    fds_[0] = -1;
  }
}

void SystemPipe::closeWriter() {
  if (fds_[1] != -1) {
    ::close(fds_[1]);
    fds_[1] = -1;
  }
}

int SystemPipe::write(const void* buf, size_t count) {
  return ::write(writerFd(), buf, count);
}

int SystemPipe::write(const std::string& msg) {
  return write(msg.data(), msg.size());
}

void SystemPipe::consume() {
  char buf[4096];
  int n;
  do n = ::read(readerFd(), buf, sizeof(buf));
  while (n > 0);
}

} // namespace xzero
