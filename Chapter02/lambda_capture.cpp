#include <algorithm>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>


auto count_value_above(const std::vector<int>& vals, int x) {
  auto is_above = [x](int v) { return v > x; };
  return std::count_if(vals.begin(), vals.end(), is_above);
}

TEST(LambdaCapture, CountValueAbove) {
  const auto vals = {1, 2, 3, 4, 5, 6};
  const auto c = count_value_above(vals, 2);
  ASSERT_EQ(4, c);
}

//
// Capture by reference vs capture by value
TEST(LambdaCapture, CaptureByValue) {
  auto vals = {1, 2, 3, 4, 5, 6};
  auto x = 3;
  auto is_above = [x](int v) { return v > x; };
  x = 4;
  auto count_b = std::count_if(vals.begin(), vals.end(), is_above);
  ASSERT_EQ(3, count_b);
}

TEST(LambdaCapture, CaptureByReference) {
  auto vals = {1, 2, 3, 4, 5, 6};
  auto x = 3;
  auto is_above = [&x](int v) { return v > x; };
  x = 4;
  auto count_b = std::count_if(vals.begin(), vals.end(), is_above);
  ASSERT_EQ(2, count_b);
}

//
// Initializating variables
TEST(LambdaCapture, Initialization) {
  auto func = [c = std::list{4, 2}]() {
    for (auto v : c) {
      std::cout << v;
    }
  };
  func();
}

// 
// Mutating member variables
TEST(LambdaCapture, MutatingCapture) {
  auto counter_func = [counter = 1]() mutable { std::cout << counter++; };

  counter_func(); // Output: 1
  counter_func(); // Output: 2
  counter_func(); // Output: 3
}

TEST(LambdaCapture, MutatingCaptureByValueVSReference) {
  // Capture by value
  {
    auto v = 7;
    auto lambda = [v]() mutable {
      std::cout << v << " ";
      ++v;
    };
    ASSERT_EQ(7, v);
    lambda();
    lambda();
    ASSERT_EQ(7, v);
    std::cout << v;
  }

  // Capture by reference
  {
    auto v = 7;
    auto lambda = [&v]() {
      std::cout << v << " ";
      ++v;
    };
    ASSERT_EQ(7, v);
    lambda();
    lambda();
    ASSERT_EQ(9, v);
    std::cout << v;
  }
}


//
// Capture all
class Foo {
public:
  auto member_function() {
    auto a = 0;
    auto b = 1.0f;

    // Capture all variables by copy
    auto lambda_0 = [=]() { std::cout << a << b; };

    // Capture all variables by reference
    auto lambda_1 = [&]() { std::cout << a << b; };

    // Capture member variables by reference
    auto lambda_2 = [this]() { std::cout << m_; };

    // Capture member variables by copy
    auto lambda_3 = [*this]() { std::cout << m_; };
  }

private:
  int m_{};
};
