#include <gtest/gtest.h>

// pow_n accepts any number type
template <typename T> 
auto pow_n(const T& v, int n) {
  auto product = T{1};
  for (int i = 0; i < n; ++i) {
    product *= v;
  }
  return product;
}

TEST(TemplateFunction, PowN) {
  auto x = pow_n<float>(2.0f, 3); // x is a float
  ASSERT_FLOAT_EQ(8.0f, x);
  ASSERT_EQ(typeid(float), typeid(x));

  auto y = pow_n<int>(3, 3); // y is an int
  ASSERT_EQ(27, y);
  ASSERT_EQ(typeid(int), typeid(y));
}
