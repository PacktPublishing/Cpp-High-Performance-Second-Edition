#if _MSC_VER
#pragma warning(disable : 4244)
#endif

#include <gtest/gtest.h>

#include <future>
#include <iostream>
#include <thread>

namespace {

int divide(int a, int b) { // No need to pass a promise ref here!
  if (b == 0) {
    throw std::runtime_error{"Divide by zero exception"};
  }
  return a / b;
}

} // namespace

TEST(Tasks, Divide) {
  std::packaged_task<decltype(divide)> task(divide);
  auto f = task.get_future();
  std::thread t(std::move(task), 45, 5);
  auto result = int{0};
  ASSERT_NO_THROW({ result = f.get(); });
  ASSERT_EQ(45 / 5, result);
  t.join();
}
