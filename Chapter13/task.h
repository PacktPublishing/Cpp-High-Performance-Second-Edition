#pragma once

#include "chapter_13.h"
#ifdef SUPPORTS_COROUTINES

#include <exception>
#include <variant>

template <typename T>
class [[nodiscard]] Task {

  struct Promise {
    std::variant<std::monostate, T, std::exception_ptr> result_;
    std::coroutine_handle<> continuation_; // Awaiting coroutine
    auto get_return_object() noexcept { return Task{*this}; }
    void return_value(T value) {
      result_.template emplace<1>(std::move(value));
    }
    void unhandled_exception() noexcept {
      result_.template emplace<2>(std::current_exception());
    }
    auto initial_suspend() { return std::suspend_always{}; }
    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        auto await_suspend(std::coroutine_handle<Promise> h) noexcept {
          return h.promise().continuation_;
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }
  };

  std::coroutine_handle<Promise> h_;
  explicit Task(Promise & p) noexcept
      : h_{std::coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;
  Task(Task && t) noexcept : h_{std::exchange(t.h_, {})} {}
  ~Task() {
    if (h_)
      h_.destroy();
  }

  // Awaitable interface
  bool await_ready() { return false; }
  auto await_suspend(std::coroutine_handle<> c) {
    h_.promise().continuation_ = c;
    return h_;
  }
  T await_resume() {
    auto&& r = h_.promise().result_;
    if (r.index() == 1) {
      return std::get<1>(std::move(r));
    } else {
      std::rethrow_exception(std::get<2>(std::move(r)));
    }
  }
};

// Template Specialization for Task<void>
template <>
class [[nodiscard]] Task<void> {

  struct Promise {
    std::exception_ptr e_;
    std::coroutine_handle<> continuation_; // Awaiting coroutine
    auto get_return_object() noexcept { return Task{*this}; }
    void return_void() {}
    void unhandled_exception() noexcept { e_ = std::current_exception(); }
    auto initial_suspend() { return std::suspend_always{}; }
    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        auto await_suspend(std::coroutine_handle<Promise> h) noexcept {
          return h.promise().continuation_;
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }
  };

  std::coroutine_handle<Promise> h_;
  explicit Task(Promise & p) noexcept
      : h_{std::coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;

  Task(Task && t) noexcept : h_{std::exchange(t.h_, {})} {}
  ~Task() {
    if (h_)
      h_.destroy();
  }

  // Awaitable interface
  bool await_ready() { return false; }
  auto await_suspend(std::coroutine_handle<> c) {
    h_.promise().continuation_ = c;
    return h_;
  }
  void await_resume() {
    if (h_.promise().e_)
      std::rethrow_exception(h_.promise().e_);
  }
};

#endif // SUPPORTS_COROUTINES