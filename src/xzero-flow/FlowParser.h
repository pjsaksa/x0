// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2017 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT
#pragma once

#include <list>
#include <vector>
#include <string>
#include <functional>

#include <xzero/defines.h>
#include <xzero-flow/FlowToken.h>
#include <xzero-flow/FlowLexer.h>
#include <xzero-flow/AST.h>  // SymbolTable
#include <xzero/Utility.h>

namespace xzero::flow {

//! \addtogroup Flow
//@{

class FlowLexer;
class NativeCallback;
class Runtime;

class FlowParser {
  std::unique_ptr<FlowLexer> lexer_;
  SymbolTable* scopeStack_;
  Runtime* runtime_;

 public:
  typedef std::function<void(const std::string&)> ErrorHandler;
  typedef std::function<bool(const std::string&, const std::string&,
                     std::vector<NativeCallback*>*)> ImportHandler;

  ErrorHandler errorHandler;
  ImportHandler importHandler;

  FlowParser() : FlowParser(nullptr, nullptr, nullptr) {}

  explicit FlowParser(
      Runtime* runtime,
      ImportHandler importHandler = nullptr,
      ErrorHandler errorHandler = nullptr);

  ~FlowParser();

  void openString(const std::string& filename);
  void openLocalFile(const std::string& filename);
  void openStream(std::unique_ptr<std::istream>&& ifs, const std::string& filename);

  std::unique_ptr<UnitSym> parse();

  Runtime* runtime() const { return runtime_; }

 private:
  class Scope;

  // error handling
  void reportUnexpectedToken();
  void reportError(const std::string& message);
  template <typename... Args> void reportError(const std::string& fmt, Args&&...);

  void reportWarning(const std::string& message);
  template <typename... Args> void reportWarning(const std::string& fmt, Args&&...);

  // lexing
  FlowToken token() const { return lexer_->token(); }
  const FlowLocation& lastLocation() { return lexer_->lastLocation(); }
  const FlowLocation& location() { return lexer_->location(); }
  const FilePos& end() const { return lexer_->lastLocation().end; }
  FlowToken nextToken() const;
  bool eof() const { return lexer_->eof(); }
  bool expect(FlowToken token);
  bool consume(FlowToken);
  bool consumeIf(FlowToken);
  bool consumeUntil(FlowToken);

  template <typename A1, typename... Args>
  bool consumeOne(A1 token, Args... tokens);
  template <typename A1>
  bool testTokens(A1 token) const;
  template <typename A1, typename... Args>
  bool testTokens(A1 token, Args... tokens) const;

  std::string stringValue() const { return lexer_->stringValue(); }
  double numberValue() const { return lexer_->numberValue(); }
  bool booleanValue() const { return lexer_->numberValue(); }

  // scoping
  SymbolTable* currentScope() { return scopeStack_; }
  SymbolTable* globalScope() const;
  std::unique_ptr<SymbolTable> enterScope(const std::string& title);
  SymbolTable* enterScope(SymbolTable* scope);
  SymbolTable* leaveScope();

  // symbol mgnt
  template <typename T>
  T* lookup(const std::string& name) {
    if (T* result = static_cast<T*>(currentScope()->lookup(name, Lookup::All)))
      return result;

    return nullptr;
  }

  template <typename T, typename... Args>
  T* createSymbol(Args&&... args) {
    return static_cast<T*>(currentScope()->appendSymbol(
        std::make_unique<T>(std::forward<Args>(args)...)));
  }

  template <typename T, typename... Args>
  T* lookupOrCreate(const std::string& name, Args&&... args) {
    if (T* result = static_cast<T*>(currentScope()->lookup(name, Lookup::All)))
      return result;

    // create symbol in global-scope
    return scopeStack_->appendSymbol(std::make_unique<T>(std::forward<Args>(args)...));
  }

  void importRuntime();
  void declareBuiltin(const NativeCallback* native);

  // syntax: decls
  std::unique_ptr<UnitSym> unit();
  bool importDecl(UnitSym* unit);
  bool importOne(std::list<std::string>& names);
  std::unique_ptr<Symbol> decl();
  std::unique_ptr<VariableSym> varDecl();
  std::unique_ptr<HandlerSym> handlerDecl(bool keyword);

  // syntax: expressions
  std::unique_ptr<Expr> expr();
  std::unique_ptr<Expr> logicExpr();   // and or xor
  std::unique_ptr<Expr> notExpr();     // not
  std::unique_ptr<Expr> relExpr();     // == != <= >= < > =^ =$ =~
  std::unique_ptr<Expr> addExpr();     // + -
  std::unique_ptr<Expr> mulExpr();     // * / shl shr
  std::unique_ptr<Expr> powExpr();     // **
  std::unique_ptr<Expr> bitNotExpr();  // ~
  std::unique_ptr<Expr> negExpr();     // -
  std::unique_ptr<Expr> primaryExpr();
  std::unique_ptr<Expr> arrayExpr();
  std::unique_ptr<Expr> literalExpr();
  std::unique_ptr<Expr> interpolatedStr();
  std::unique_ptr<Expr> castExpr();

  std::unique_ptr<ParamList> paramList();
  std::unique_ptr<Expr> namedExpr(std::string* name);

  // syntax: statements
  std::unique_ptr<Stmt> stmt();
  std::unique_ptr<Stmt> ifStmt();
  std::unique_ptr<Stmt> matchStmt();
  std::unique_ptr<Stmt> compoundStmt();
  std::unique_ptr<Stmt> identStmt();
  std::unique_ptr<CallExpr> callStmt(const std::list<Symbol*>& callables);
  std::unique_ptr<CallExpr> resolve(const std::list<CallableSym*>& symbols,
                                    ParamList&& params);
  std::unique_ptr<Stmt> postscriptStmt(std::unique_ptr<Stmt> baseStmt);
};

// {{{ inlines
template <typename... Args>
inline void FlowParser::reportError(const std::string& fmt, Args&&... args) {
  char buf[1024];
  snprintf(buf, sizeof(buf), fmt.c_str(), args...);
  reportError(buf);
}

template <typename... Args>
inline void FlowParser::reportWarning(const std::string& fmt, Args&&... args) {
  char buf[1024];
  snprintf(buf, sizeof(buf), fmt.c_str(), args...);
  reportWarning(buf);
}

template <typename A1, typename... Args>
inline bool FlowParser::consumeOne(A1 a1, Args... tokens) {
  if (!testTokens(a1, tokens...)) {
    reportUnexpectedToken();
    return false;
  }

  nextToken();
  return true;
}

template <typename A1>
inline bool FlowParser::testTokens(A1 a1) const {
  return token() == a1;
}

template <typename A1, typename... Args>
inline bool FlowParser::testTokens(A1 a1, Args... tokens) const {
  if (token() == a1) return true;

  return testTokens(tokens...);
}

// }}}

//!@}

}  // namespace xzero::flow
