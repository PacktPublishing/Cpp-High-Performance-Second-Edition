#include <gtest/gtest.h>

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

std::atomic<int> counter;

auto increment_counter(int n) {
  for (int i = 0; i < n; i++) {
    ++counter;
  }
}

} // namespace

TEST(CounterAtomic, IncrementCounter) {
  const int n_times = 1000000;
  std::thread t1(increment_counter, n_times);
  std::thread t2(increment_counter, n_times);

  t1.join();
  t2.join();
  std::cout << counter << '\n';

  // If we don't have a data race, this assert should hold:
  ASSERT_EQ(n_times * 2, counter);
}
