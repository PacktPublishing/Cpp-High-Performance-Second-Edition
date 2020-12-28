#include <gtest/gtest.h>

#include <future>
#include <iostream>
#include <thread>

namespace {

auto divide(int a, int b, std::promise<int>& p) {
  if (b == 0) {
    const auto e = std::runtime_error("Divide by zero exception");
    p.set_exception(std::make_exception_ptr(e));
  } else {
    const auto result = a / b;
    p.set_value(result);
  }
}

} // namespace

TEST(FutureAndPromises, Divide) {
  std::promise<int> p;
  std::thread t{divide, 45, 5, std::ref(p)};

  auto f = p.get_future();
  auto result = int{0};
  ASSERT_NO_THROW({ result = f.get(); });
  ASSERT_EQ(45 / 5, result);

  t.join();
}
