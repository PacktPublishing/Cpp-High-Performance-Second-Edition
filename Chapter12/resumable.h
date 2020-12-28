#pragma once 

#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <exception>
#include <utility>

class Resumable { // The return object
  struct Promise {
    auto get_return_object() {
      using Handle = std::coroutine_handle<Promise>;
      return Resumable{Handle::from_promise(*this)};
    }
    auto initial_suspend() { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };
  std::coroutine_handle<Promise> h_;
  explicit Resumable(std::coroutine_handle<Promise> h) : h_{h} {}

public:
  using promise_type = Promise;
  Resumable(Resumable&& r) : h_{std::exchange(r.h_, {})} {}
  ~Resumable() {
    if (h_) {
      h_.destroy();
    }
  }
  bool resume() {
    if (!h_.done()) {
      h_.resume();
    }
    return !h_.done();
  }
};

#endif // SUPPORTS_COROUTINES