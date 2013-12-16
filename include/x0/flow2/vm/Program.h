#pragma once

#include <x0/Api.h>
#include <x0/flow2/vm/Instruction.h>
#include <x0/flow2/FlowType.h>           // FlowNumber

#include <vector>
#include <utility>
#include <memory>

namespace x0 {
namespace FlowVM {

class Runner;
class Runtime;
class Handler;
class NativeCallback;

class X0_API Program
{
public:
    Program();
    Program(
        const std::vector<FlowNumber>& constNumbers,
        const std::vector<std::string>& constStrings,
        const std::vector<std::string>& regularExpressions,
        const std::vector<std::pair<std::string, std::string>>& modules,
        const std::vector<std::string>& nativeHandlerSignatures,
        const std::vector<std::string>& nativeFunctionSignatures
    );
    Program(Program&) = delete;
    Program& operator=(Program&) = delete;
    ~Program();

    inline const std::vector<FlowNumber>& numbers() const { return numbers_; }
    inline const std::vector<FlowString>& strings() const { return strings_; }
    inline const std::vector<std::string>& regularExpressions() const { return regularExpressions_; }
    inline const std::vector<Handler*> handlers() const { return handlers_; }

    Handler* createHandler(const std::string& name);
    Handler* createHandler(const std::string& name, const std::vector<FlowVM::Instruction>& instructions);
    Handler* findHandler(const std::string& name) const;
    Handler* handler(size_t index) const { return handlers_[index]; }
    int indexOf(const Handler* handler) const;

    NativeCallback* nativeHandler(size_t id) const { return nativeHandlers_[id]; }
    NativeCallback* nativeFunction(size_t id) const { return nativeFunctions_[id]; }

    bool link(Runtime* runtime);

    void dump();

private:
    std::vector<FlowNumber> numbers_;
    std::vector<FlowString> strings_;
    std::vector<std::string> regularExpressions_;               // XXX to be a pre-compiled handled during runtime
    std::vector<std::pair<std::string, std::string>> modules_;
    std::vector<std::string> nativeHandlerSignatures_;
    std::vector<std::string> nativeFunctionSignatures_;

    std::vector<NativeCallback*> nativeHandlers_;
    std::vector<NativeCallback*> nativeFunctions_;
    std::vector<Handler*> handlers_;
    Runtime* runtime_;
};

} // namespace FlowVM
} // namespace x0
