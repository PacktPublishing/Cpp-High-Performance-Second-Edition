#include <gtest/gtest.h>

TEST(LambdaGeneric, BasicExample) {
  auto v = 3; // int
  auto lambda = [v](auto v0, auto v1) { return v + v0 * v1; };

  auto res_int = lambda(1, 2);
  ASSERT_EQ(5, res_int);
  auto res_float = lambda(1.0f, 2.0f);
  ASSERT_NEAR(5.0f, res_float, 0.1f);
}

TEST(LambdaGeneric, AutoAndTypename) {
  // Using auto
  auto x = [](auto v) { return v + 1; };

  // Using typename
  auto y = []<typename Val>(Val v) { return v + 1; };

  {
    // Using char
    auto v = 'a';
    ASSERT_EQ(v + 1, x(v));
    ASSERT_EQ(v + 1, y(v));
  }

  {
    // Using int
    ASSERT_EQ(42, x(41));
    ASSERT_EQ(42, y(41));
  }
  {
    // Using
    auto v = std::uint64_t{41};
    ASSERT_EQ(v + 1, x(v));
    ASSERT_EQ(v + 1, y(v));
  }
}
