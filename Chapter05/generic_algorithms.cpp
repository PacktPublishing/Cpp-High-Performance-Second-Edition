// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges) && defined(__cpp_concepts) && defined(__cpp_lib_concepts)

#include <gtest/gtest.h>

#include <algorithm>
#include <concepts>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <vector>

namespace {

template <typename Iterator, typename T>
auto contains(Iterator begin, Iterator end, const T& x) {
  for (auto it = begin; it != end; ++it) {
    if (*it == x) {
      return true;
    }
  }
  return false;
}

auto contains(const std::ranges::range auto& r, const auto& x) {
  auto it = std::begin(r);
  auto sentinel = std::end(r);
  return contains(it, sentinel, x);
}

} // namespace

TEST(GenericAlgorithms, Contains) {

  auto v = std::vector{3, 4, 2, 4};
  if (contains(v.begin(), v.end(), 3)) {
    // Found the value...
  }

  ASSERT_TRUE(contains(v.begin(), v.end(), 3));
  ASSERT_TRUE(contains(v.begin(), v.end(), 4));
  ASSERT_TRUE(contains(v.begin(), v.end(), 2));
  ASSERT_FALSE(contains(v.begin(), v.end(), 1));
}


TEST(GenericAlgorithms, ContainsWithRanges) {

  auto v = std::vector{3, 4, 2, 4};
  if (contains(v, 3)) {
    // Found the value...
  }

  ASSERT_TRUE(contains(v, 3));
  ASSERT_TRUE(contains(v, 4));
  ASSERT_TRUE(contains(v, 2));
  ASSERT_FALSE(contains(v, 1));
}

#endif // ranges && concepts