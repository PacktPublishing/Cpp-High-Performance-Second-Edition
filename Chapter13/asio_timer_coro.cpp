#include "chapter_13.h"
#ifdef SUPPORTS_COROUTINES

#include <gtest/gtest.h>

#include "task.h"
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono;
namespace asio = boost::asio;

namespace {

template <typename R, typename P>
auto async_await(asio::io_context& ctx, std::chrono::duration<R, P> d) {
  struct Awaitable {
    asio::system_timer t_;
    std::chrono::duration<R, P> d_;
    boost::system::error_code ec_{};

    bool await_ready() { return d_.count() <= 0; }
    void await_suspend(std::coroutine_handle<> h) {
      t_.expires_from_now(d_);
      t_.async_wait([this, h](auto e) mutable {
        this->ec_ = e;
        h.resume();
      });
    }
    void await_resume() {
      if (ec_) throw boost::system::system_error(ec_);
    }
  };
  return Awaitable{asio::system_timer{ctx}, d};
}

} // namespace

namespace {

struct Detached {
  struct promise_type {
    auto get_return_object() { return Detached{}; }
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_never{}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };
};

Detached run_task_impl(asio::io_context& ctx, Task<void>&& t) {
  auto wg = asio::executor_work_guard{ctx.get_executor()};
  co_await t;
}

void run_task(asio::io_context& ctx, Task<void>&& t) {
  run_task_impl(ctx, std::move(t));
  ctx.run();
}


Task<void> test_sleep(asio::io_context& ctx) {
  std::cout << "Hello!  ";
  co_await async_await(ctx, 100ms);
  std::cout << "Delayed output\n";
}

} // namespace

TEST(BoostAsioCoro, TimerExampleUsingCoroutines) {

  auto ctx = asio::io_context{};
  auto task = test_sleep(ctx);

  run_task(ctx, std::move(task));  
};

#endif // SUPPORTS_COROUTINES