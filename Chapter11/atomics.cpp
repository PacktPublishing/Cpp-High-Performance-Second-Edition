// No compiler support yet
#include <version>

#if defined(__cpp_lib_atomic_wait) && defined(__cpp_lib_atomic_flag_test)

#include <gtest/gtest.h>

#include <atomic>
#include <iostream>
#include <random>
#include <thread>

namespace SpinLock {

class SimpleMutex {
  std::atomic_flag is_locked{}; // Cleared by default
public:
  auto lock() noexcept {
    while (is_locked.test_and_set()) {
      while (is_locked.test()) // C++20
        ;                      // Spin here
    }
  }
  auto unlock() noexcept { is_locked.clear(); }
};

TEST(Atomics, SimpleSpinLock) {

  constexpr auto n = 1'000'000;
  auto counter = 0; // Counter will be protected by mutex
  auto counter_mutex = SimpleMutex{};

  auto increment_counter = [&] {
    for (int i = 0; i < n; ++i) {
      counter_mutex.lock();
      ++counter;
      counter_mutex.unlock();
    }
  };

  auto t1 = std::thread{increment_counter};
  auto t2 = std::thread{increment_counter};

  t1.join(); // Or use std::jthread
  t2.join();

  std::cout << counter << '\n';
  // If we don't have a data race, this assert should hold:
  ASSERT_EQ(n * 2, counter);
}

} // namespace SpinLock

namespace WaitAndNotify {

class SimpleMutex {
  std::atomic_flag is_locked{};

public:
  auto lock() noexcept {
    while (is_locked.test_and_set())
      is_locked.wait(true); // Don’t spin, wait
  }

  auto unlock() noexcept {
    is_locked.clear();
    is_locked.notify_one(); // Notify blocked thread
  }
};

TEST(Atomics, WaitAndNotify) {

  constexpr auto n = 1000000;
  auto counter = 0; // Counter will be protected by mutex
  auto counter_mutex = SimpleMutex{};

  auto increment_counter = [&] {
    for (int i = 0; i < n; ++i) {
      counter_mutex.lock();
      ++counter;
      counter_mutex.unlock();
    }
  };

  auto t1 = std::thread{increment_counter};
  auto t2 = std::thread{increment_counter};
  t1.join();
  t2.join();

  std::cout << counter << '\n';
  // If we don't have a data race, this assert should hold:
  ASSERT_EQ(n * 2, counter);
}

} // namespace WaitAndNotify

#endif // atomic wait && atomic notify