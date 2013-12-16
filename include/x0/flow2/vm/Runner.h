#pragma once

#include <x0/flow2/FlowType.h>
#include <x0/flow2/vm/Handler.h>
#include <x0/flow2/vm/Instruction.h>
#include <utility>
#include <list>
#include <memory>
#include <new>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <string>

namespace x0 {
namespace FlowVM {

// ExecutionEngine
// VM
class Runner
{
private:
    Handler* handler_;
    Program* program_;
    void* userdata_;

    std::list<std::string> stringGarbage_;

    Register data_[];

public:
    static std::unique_ptr<Runner> create(Handler* handler);
    static void operator delete (void* p);

    bool run();

    Handler* handler() const { return handler_; }
    Program* program() const { return program_; }
    void* userdata() const { return userdata_; }
    void setUserData(void* p) { userdata_ = p; }

    FlowString* createString(const std::string& value);

private:
    explicit Runner(Handler* handler);
    Runner(Runner&) = delete;
    Runner& operator=(Runner&) = delete;
};

} // namespace FlowVM
} // namespace x0
