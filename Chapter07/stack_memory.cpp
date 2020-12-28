#include <iostream>
#include <gtest/gtest.h>



void func1() {
  auto i = 0;
  std::cout << "func1(): " << std::addressof(i) << '\n';
}

void func2() {
  auto i = 0;
  std::cout << "func2(): " << std::addressof(i) << '\n';
  func1();
}

TEST(StackMemory, StackFrameAddress) {
  auto i = 0;
  std::cout << "main():  " << std::addressof(i) << '\n';
  func2();
  func1();
}
