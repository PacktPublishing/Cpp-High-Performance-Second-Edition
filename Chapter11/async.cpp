#include <gtest/gtest.h>

#include <atomic>
#include <complex>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

namespace {

int divide(int a, int b) {
  if (b == 0) {
    throw std::runtime_error("Divide by zero exception");
  }
  return a / b;
}

} // namespace

TEST(Async, Divide) {
  auto f = std::async(divide, 45, 5);
  auto result = int{0};
  ASSERT_NO_THROW({ result = f.get(); });
  ASSERT_EQ(45 / 5, result);
}
