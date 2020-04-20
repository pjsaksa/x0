// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2020 Pauli Saksa
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include <xzero/WallClock.h>
#include <xzero/executor/FemcScheduler.h>
#include <xzero/net/Socket.h>
#include <xzero/thread/Wakeup.h>

extern "C" {
#include <femc/dispatcher.h>
}

#include <utility>

using xzero::FemcScheduler;

//

namespace
{
    using CancellableTask = std::pair< FemcScheduler::Task,
                                       FemcScheduler::HandleRef >;

    // -----

    bool runCancellableTask(void* cTask_void, int)
    {
        CancellableTask* cTask = static_cast<CancellableTask*>( cTask_void );

        if (cTask) {
            cTask->second->fire( cTask->first );
            delete cTask;
        }

        return true;
    };

    // -----

    class TimedIoService {
    public:
        using Duration  = xzero::Duration;
        using Handle    = FemcScheduler::Handle;
        using HandleRef = FemcScheduler::HandleRef;
        using Task      = FemcScheduler::Task;

        //

        TimedIoService(int fd, Task ioTask, Task timeoutTask, Duration timeout)
            : fd_(fd),
              handle_(std::make_shared<Handle>()),
              ioTask_(ioTask),
              timeout_(timeout),
              timeoutTask_(timeout_ > Duration(0) ? timeoutTask : nullptr)
        {
            if (timeoutTask_) {
                fdd_add_timer(static_onTimeout, static_cast<TimedIoService*>( this ), fd, timeout_.milliseconds(), 0);
            }
        }

        virtual ~TimedIoService() = default;

        HandleRef handle()
        {
            return handle_;
        }

        void fireTask(Task& task)
        {
            HandleRef cache;
            handle_.swap(cache);            // 1. reset handle_

            if (cache) {
                cache->fire( task );        // 2. fire ioTask_
            }
        }

    protected:
        int fd_;
        HandleRef handle_;

        //

        virtual void deactivateIo() = 0;

        //

        static bool static_onIo(void* service_void, int fd)
        {
            TimedIoService* service = static_cast<TimedIoService*>( service_void );
            if (service) {
                service->deactivateIo();
                service->fireTask( service->ioTask_ );

                if ( !service->timeoutTask_ ) {
                    delete service;
                }
            }
            return true;
        }

        static bool static_onTimeout(void* service_void, int fd)
        {
            TimedIoService* service = static_cast<TimedIoService*>( service_void );
            if (service) {
                service->deactivateIo();
                service->fireTask( service->timeoutTask_ );
                delete service;
            }
            return true;
        }

    private:
        Task ioTask_;
        Duration timeout_;
        Task timeoutTask_;
    };

    // -----

    class TimedInputService : public TimedIoService {
    public:
        TimedInputService(int fd, Task ioTask, Task timeoutTask, Duration timeout)
            : TimedIoService(fd,
                             std::move( ioTask ),
                             std::move( timeoutTask ),
                             timeout)
        {
            fdd_init_service_input(&inputService_, static_cast<TimedIoService*>( this ), &TimedIoService::static_onIo);
            fdd_add_input(fd_, &inputService_);
        }

        ~TimedInputService() override
        {
            deactivateIo();
        }

    protected:
        void deactivateIo() override
        {
            if (fd_ >= 0) {
                fdd_remove_input(fd_);
                fd_ = -1;
            }
        }

    private:
        fdd_service_input inputService_;
    };

    // -----

    class TimedOutputService : public TimedIoService {
    public:
        TimedOutputService(int fd, Task ioTask, Task timeoutTask, Duration timeout)
            : TimedIoService(fd,
                             std::move( ioTask ),
                             std::move( timeoutTask ),
                             timeout)
        {
            fdd_init_service_output(&outputService_, static_cast<TimedIoService*>( this ), &TimedIoService::static_onIo);
            fdd_add_output(fd_, &outputService_);
        }

        ~TimedOutputService() override
        {
            deactivateIo();
        }

    protected:
        void deactivateIo() override
        {
            if (fd_ >= 0) {
                fdd_remove_output(fd_);
                fd_ = -1;
            }
        }

    private:
        fdd_service_output outputService_;
    };
}

// ------------------------------------------------------------

FemcScheduler::FemcScheduler(ExceptionHandler eh)
    : EventLoop{std::move(eh)}
{
}

FemcScheduler::FemcScheduler()
    : FemcScheduler(CatchAndLogExceptionHandler("FemcScheduler"))
{
}

std::string FemcScheduler::toString() const
{
    return "FemcScheduler{}";
}

void FemcScheduler::execute(Task task)
{
    fdd_add_timer([] (void* task_void, int) -> bool
                  {
                      Task* task = static_cast<Task*>( task_void );
                      if (task) {
                          (*task)();
                          delete task;
                      }
                      return true;
                  },
                  new Task(std::move(task)),
                  0,
                  0,
                  0);
}

FemcScheduler::HandleRef FemcScheduler::executeOnReadable(const Socket& s, Task inputTask, Duration timeout, Task timeoutTask)
{
    TimedInputService& tis = *new TimedInputService(s.native(), inputTask, timeoutTask, timeout);

    return tis.handle();
}

FemcScheduler::HandleRef FemcScheduler::executeOnWritable(const Socket& s, Task outputTask, Duration timeout, Task timeoutTask)
{
    TimedOutputService& tos = *new TimedOutputService(s.native(), outputTask, timeoutTask, timeout);

    return tos.handle();
}

FemcScheduler::HandleRef FemcScheduler::executeAfter(Duration delay, Task task)
{
    CancellableTask* cTask = new CancellableTask(task,
                                                 std::make_shared<Handle>());

    fdd_add_timer(&runCancellableTask,
                  cTask,
                  0,
                  delay.milliseconds(),
                  0);

    return cTask->second;
}

FemcScheduler::HandleRef FemcScheduler::executeAt(UnixTime when, Task task)
{
    return executeAfter(when - WallClock::now(),
                        task);
}

void FemcScheduler::executeOnWakeup(Task task, Wakeup* wakeup, long generation)
{
    wakeup->onWakeup(generation,
                     [&sched = *this, task] ()
                     {
                         sched.execute(task);
                     });
}

void FemcScheduler::runLoop()
{
    fdd_main(FDD_INFINITE);
}

void FemcScheduler::runLoopOnce()
{
    fdd_main(0);
}

void FemcScheduler::breakLoop()
{
    fdd_shutdown();
}
