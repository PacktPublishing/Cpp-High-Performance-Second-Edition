// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>
#include <span>
#include <string>
#include <vector>

namespace {

auto reset(std::span<int> values, int n) {
  for (auto& i : std::ranges::take_view{values, n}) {
    i = int{};
  }
}

} // namespace

TEST(Span, ResetArray) {
  int a[]{33, 44, 55, 66, 77};
  reset(a, 3);
  // a is now [0, 0, 0, 66, 77]

  int result[]{0, 0, 0, 66, 77};
  ASSERT_EQ(sizeof(result), sizeof(a));
  const int n = sizeof(a) / sizeof(*a);
  ASSERT_TRUE(std::equal(a, a + n, result));
}

TEST(Span, ResetVector) {

  auto v = std::vector{33, 44, 55, 66, 77};
  reset(v, 2);
  // v is now [0, 0, 55, 66, 77]

  auto result = std::vector{0, 0, 55, 66, 77};
  ASSERT_EQ(v, result);
}

#endif // ranges