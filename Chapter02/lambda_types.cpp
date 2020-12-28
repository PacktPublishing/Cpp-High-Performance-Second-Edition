#include <gtest/gtest.h>

// Requires C++20
TEST(LambdaTypes, StatelessLambdas) {
  auto x = [] {}; // A lambda without captures
  auto y = x;     // Assignable
  decltype(y) z;  // Default-constructible
  static_assert(std::is_same_v<decltype(x), decltype(y)>); // passes
  static_assert(std::is_same_v<decltype(x), decltype(z)>); // passes
}