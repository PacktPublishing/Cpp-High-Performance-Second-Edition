#include <gtest/gtest.h>

template <int N, typename T> 
auto const_pow_n(const T& v) {
  auto product = T{1};
  for (int i = 0; i < N; ++i) {
    product *= v;
  }
  return product;
}

// Template speicalization using:
// N = 2
// T = int
template <> 
auto const_pow_n<2, int>(const int& v) { 
  return v * v; 
}

TEST(IntegerAsTemplateParameter, ConstPowN) {
  // The compiler generates a function which squares the value
  auto x2 = const_pow_n<2>(4.0f); // Square
  ASSERT_FLOAT_EQ(16.0f, x2);

  // The compiler generates a function which cubes the value
  auto x3 = const_pow_n<3>(4.0f); // Cube
  ASSERT_FLOAT_EQ(64.0f, x3);
}

TEST(IntegerAsTempalteParameter, TemplateSpecialization) {

  auto x2 = const_pow_n<2>(4); // Uses template specialization
  ASSERT_EQ(16, x2);

  auto x3 = const_pow_n<3>(4); // Uses generic template
  ASSERT_EQ(64, x3);
}
