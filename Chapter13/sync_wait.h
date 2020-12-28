#pragma once

#include "chapter_13.h"
#ifdef SUPPORTS_COROUTINES

#include <cassert>
#include <version>

#if defined(__cpp_lib_semaphore)
#include <semaphore>
#else
// Fallback if your standard lib doesn't provide <semaphore> yet
#include <condition_variable>
#include <mutex>
#endif

#include "task.h"

namespace detail {

template <typename T>
class SyncWaitTask { // A helper class only used by sync_wait()

  struct Promise {
    SyncWaitTask get_return_object() noexcept { return SyncWaitTask{*this}; }
    auto initial_suspend() noexcept { return std::suspend_always{}; }
    void unhandled_exception() noexcept { error_ = std::current_exception(); }
    auto yield_value(T&& x) noexcept { // Result has arrived
      value_ = std::addressof(x);
      return final_suspend();
    }

    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        void await_suspend(std::coroutine_handle<Promise> h) noexcept {

#if defined(__cpp_lib_semaphore)
          h.promise().semaphore_.release(); // Signal!
#else
          {
            auto lock = std::unique_lock{h.promise().mtx_};
            h.promise().ready_ = true;
          }
          h.promise().cv_.notify_one();
#endif
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }

    void return_void() noexcept { assert(false); }

    T* value_ = nullptr;
    std::exception_ptr error_;

#if defined(__cpp_lib_semaphore)
    std::binary_semaphore semaphore_;
#else
    bool ready_{false};
    std::mutex mtx_;
    std::condition_variable cv_;
#endif
  };

  std::coroutine_handle<Promise> h_;
  explicit SyncWaitTask(Promise& p) noexcept
      : h_{std::coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;
  SyncWaitTask(SyncWaitTask&& t) noexcept : h_{std::exchange(t.h_, {})} {}
  ~SyncWaitTask() {
    if (h_)
      h_.destroy();
  }

  // Called from sync_wait(). Will block and retrieve the
  // value or error from the task passed to sync_wait()
  T&& get() {
    auto& p = h_.promise();
    h_.resume();

#if defined(__cpp_lib_semaphore)
    p.semaphore_.acquire(); // Block until signal
#else
    {
      auto lock = std::unique_lock{p.mtx_};
      p.cv_.wait(lock, [&p] { return p.ready_; });
    }
#endif

    if (p.error_)
      std::rethrow_exception(p.error_);
    return static_cast<T&&>(*p.value_);
  }
};

} // namespace detail

template <typename T>
using Result = decltype(std::declval<T&>().await_resume());

template <typename T>
Result<T> sync_wait(T&& task) {
  if constexpr (std::is_void_v<Result<T>>) {
    struct Empty {};
    auto coro = [&]() -> detail::SyncWaitTask<Empty> {
      co_await std::forward<T>(task);
      co_yield Empty{};
      assert(false); // Coroutine will be destroyed
    };               // before it has a chance to return.
    (void)coro().get();
    return; // Result<T> is void
  } else {
    auto coro = [&]() -> detail::SyncWaitTask<Result<T>> {
      co_yield co_await std::forward<T>(task);
      assert(false);
    };
    return coro().get(); // Rerturn value
  }
}

#endif // SUPPORTS_COROUTINES