#include <iostream>
#include <gtest/gtest.h>

// This program will crash once we run out of stack memory

// Since there is no base case
auto func(std::byte* stack_bottom_addr) -> void {
  std::byte data[1024];
  std::cout << stack_bottom_addr - data << '\n';
  func(stack_bottom_addr);
}

TEST(StackSize, StackOverflow) {
  // gtest will start separate process to execute code in block
  ASSERT_DEATH_IF_SUPPORTED({
    std::byte b;
    func(&b);
  }, "");
}
