// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2020 Pauli Saksa
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/executor/EventLoop.h>

#include <string>

namespace xzero
{
    class FemcScheduler : public EventLoop {
    public:
        using EventLoop::executeOnReadable;
        using EventLoop::executeOnWritable;

        //

        FemcScheduler();
        explicit FemcScheduler(ExceptionHandler eh);

        FemcScheduler(const FemcScheduler&) = delete;
        FemcScheduler& operator=(const FemcScheduler&) = delete;

        FemcScheduler(FemcScheduler&&) = default;
        FemcScheduler& operator=(FemcScheduler&&) = default;
        ~FemcScheduler() override = default;

        //

        std::string toString() const override;

        void execute(Task task) override;

        HandleRef executeOnReadable(const Socket& s, Task task, Duration timeout, Task onTimeout) override;
        HandleRef executeOnWritable(const Socket& s, Task task, Duration timeout, Task onTimeout) override;

        HandleRef executeAfter(Duration delay, Task task) override;
        HandleRef executeAt(UnixTime when, Task task) override;
        void executeOnWakeup(Task task, Wakeup* wakeup, long generation) override;

        void runLoop() override;
        void runLoopOnce() override;

        void breakLoop() override;
    };
}
