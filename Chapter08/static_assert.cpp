
#include <gtest/gtest.h>

namespace {

template <int N, typename T> //, int N>
auto const_pow_n(const T& v) {
  static_assert(N >= 0, "N must be positive");
  auto product = T{1};
  for (int i = 0; i < N; ++i) {
    product *= v;
  }
  return product;
}

} // namespace

TEST(StaticAssert, ConstPowN) {

  auto x = const_pow_n<5>(2); // Compiles, N is positive
  // auto y = const_pow_n<int, -1>(2); // Does not compile, N is negative

  ASSERT_EQ(x, 32);
}