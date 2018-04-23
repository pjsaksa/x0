// This file is part of the "x0" project, http://github.com/christianparpart/x0>
//   (c) 2009-2018 Christian Parpart <christian@parpart.family>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/Api.h>
#include <xzero/Result.h>
#include <xzero/Duration.h>
#include <xzero/RuntimeError.h>
#include <xzero/thread/Wakeup.h>
#include <functional>
#include <memory>
#include <mutex>
#include <cstdlib>
#include <system_error>

namespace xzero {

class Scheduler;

template <typename> class Future;
template <typename> class Promise;

enum class PromiseStatus {
  UNDEFINED = 0,
  SUCCESS,
  FAILURE,
};

inline bool operator!(PromiseStatus s) {
  return s == PromiseStatus::UNDEFINED;
}

template <typename T>
class PromiseState : public std::enable_shared_from_this<PromiseState<T>> {
 public:
  PromiseState();
  ~PromiseState();

 private:
  Wakeup wakeup;
  std::error_code error;
  std::mutex mutex; // FIXPAUL use spinlock
  char value_data[sizeof(T)];
  PromiseStatus status;

  std::function<void (std::error_code status)> on_failure;
  std::function<void (T& value)> on_success;

  friend class Future<T>;
  friend class Promise<T>;

  T& value() { return *((T*) value_data); }
};

template <>
class PromiseState<void> : public std::enable_shared_from_this<PromiseState<void>> {
 public:
  PromiseState();
  ~PromiseState();

 private:
  Wakeup wakeup;
  std::error_code error;
  std::mutex mutex; // FIXPAUL use spinlock
  PromiseStatus status;

  std::function<void (std::error_code status)> on_failure;
  std::function<void ()> on_success;

  friend class Future<void>;
  friend class Promise<void>;
};

template <typename T>
class Future {
 public:
  typedef T value_type;

  Future(std::shared_ptr<PromiseState<T>> promise_state);
  Future(const Future<T>& other);
  Future(Future<T>&& other);
  ~Future();

  Future& operator=(const Future<T>& other);

  bool isReady() const;
  bool isFailure() const;
  bool isSuccess() const;

  void onSuccess(std::function<void (T& value)> fn);
  void onFailure(std::function<void (std::error_code ec)> fn);

  template<typename U> void onFailure(Promise<U> forward);
  template<typename U> void onSuccess(Promise<U> forward);

  void wait() const;
  void wait(const Duration& timeout) const;

  void onReady(std::function<void> fn);
  void onReady(Scheduler* scheduler, std::function<void> fn);

  T& get();
  const T& get() const;
  const std::error_code& error() const;
  const T& waitAndGet() const;
  Result<T> waitAndGetResult() const;

  Wakeup* wakeup() const;

  /**
   * Chains this @c Future<T> with another @c Continuation function that
   * returns a @c Future<R> as well.
   *
   * @param cont a continuation function or lambda that receives the
   *             result of this @c Future<T> and returns another Future.
   *
   * @return a Future that corresponds to the passed Continuation.
   *
   * Any error code will be chained on the the returned Future.
   */
  template<typename Continuation>
  auto chain(Continuation cont) -> decltype(cont(T())) {
    Promise<typename decltype(cont(T()))::value_type> promise;
    onFailure([promise](std::error_code ec) { promise.failure(ec); });
    onSuccess([promise, cont](auto val) {
        auto y = cont(val);
        y.onSuccess([=](auto& yval) { promise.success(yval); });
        y.onFailure([=](std::error_code ec) { promise.failure(ec); });
    });
    return promise.future();
  }

 protected:
  std::shared_ptr<PromiseState<T>> state_;
};

template<>
class Future<void> {
 public:
  typedef void value_type;

  Future(std::shared_ptr<PromiseState<value_type>> promise_state);
  Future(const Future<value_type>& other);
  Future(Future<value_type>&& other);
  ~Future();

  Future& operator=(const Future<value_type>& other);

  bool isReady() const;
  bool isFailure() const;
  bool isSuccess() const;

  void onSuccess(std::function<void()> fn);
  void onFailure(std::function<void(std::error_code ec)> fn);

  template<typename U> void onSuccess(Promise<U> forward);
  template<typename U> void onFailure(Promise<U> forward);

  void wait() const;
  void wait(const Duration& timeout) const;

  const std::error_code& error() const;

  Wakeup* wakeup() const;

  /**
   * Chains this @c Future<T> with another @c Continuation function that
   * returns a @c Future<R> as well.
   *
   * @param cont a continuation function or lambda that receives the
   *             result of this @c Future<T> and returns another Future.
   *
   * @return a Future that corresponds to the passed Continuation.
   *
   * Any error code will be chained on the the returned Future.
   */
  template<typename Continuation>
  auto chain(Continuation cont) -> decltype(cont()) {
    Promise<typename decltype(cont())::value_type> promise;
    onFailure([promise](std::error_code ec) { promise.failure(ec); });
    onSuccess([promise, cont]() {
        auto y = cont();
        y.onSuccess([=](const auto& yval) { promise.success(yval); });
        y.onFailure([=](std::error_code ec) { promise.failure(ec); });
    });
    return promise.future();
  }

 protected:
  std::shared_ptr<PromiseState<value_type>> state_;
};

template <typename T>
class Promise {
 public:
  Promise();
  Promise(const Promise<T>& other);
  Promise(Promise<T>&& other);
  ~Promise();

  Promise<T>& operator=(const Promise<T>& other);
  Promise<T>& operator=(Promise&& other);

  void success(const T& value) const;
  void success(T&& value) const;
  void failure(const std::error_code& ec) const;
  void failure(std::errc ec) const;

  Future<T> future() const;
  bool isFulfilled() const;

 protected:
  mutable std::shared_ptr<PromiseState<T>> state_;
};

template <>
class Promise<void> {
 public:
  typedef void value_type;

  Promise();
  Promise(const Promise<value_type>& other);
  Promise(Promise<value_type>&& other);
  ~Promise();

  Promise<value_type>& operator=(const Promise<value_type>& other);
  Promise<value_type>& operator=(Promise<value_type>&& other);

  void success() const;
  void failure(const std::error_code& ec) const;
  void failure(std::errc ec) const;

  Future<value_type> future() const;
  bool isFulfilled() const;

 protected:
  mutable std::shared_ptr<PromiseState<value_type>> state_;
};
} // namespace xzero

#include <xzero/thread/Future-impl.h>
