#include <iostream>
#include <gtest/gtest.h>

// See operator new() and operator delete() implementation in operator_new.cpp file
extern size_t allocated;

auto print_string_mem(const char* chars) {
  allocated = 0;
  auto s = std::string(chars);
  std::cout << "stack space = " << sizeof(s)
    << ", heap space = " << allocated
    << ", capacity = " << s.capacity() << '\n';
}

class SmallSizeOptimization : public ::testing::Test {
protected:
  void SetUp() override {
    allocated = 0;
  }
};

TEST_F(SmallSizeOptimization, StringMemory) {
   // Elaborate with different string sizes
  print_string_mem("");
  print_string_mem("1234567890123456789012");
  print_string_mem("12345678901234567890123");
}
