#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <gtest/gtest.h>

#include "resumable.h"

#include <limits>
#include <thread>


namespace {

Resumable coroutine() { // Same as before
  std::cout << "c1 ";
  co_await std::suspend_always{};
  std::cout << "c2 ";
}

auto coro_factory() { // Create and return a coroutine
  auto res = coroutine();
  return res;
}

} // namespace

TEST(PassingCoroutinesAround, ResumeFromDifferentThread) {
  auto r = coro_factory();
  r.resume(); // Resume from main

  auto t = std::thread{[r = std::move(r)]() mutable {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    r.resume(); // Resume from thread
  }};
  t.join();
}


#endif // SUPPORTS_COROUTINES