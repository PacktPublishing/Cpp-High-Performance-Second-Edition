// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include "to_vector.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <list>
#include <ranges>
#include <string>
#include <vector>


TEST(MaterializeViews, IntListToStringVector) {

  auto ints = std::list{2, 3, 4, 2, 1};
  auto r =
      ints | std::views::transform([](auto i) { return std::to_string(i); });
  auto strings = to_vector(r);

  auto result = std::vector<std::string>{"2", "3", "4", "2", "1"};
  ASSERT_EQ(strings, result);
}

TEST(MaterializeViews, IteratorQualityNotPreserved) {

  auto vec = std::vector{4, 2, 7, 1, 2, 6, 1, 5};
  auto is_odd = [](auto i) { return i % 2 == 1; };
  auto odd_numbers = vec | std::views::filter(is_odd);

  // Doesn't compile, sort() requires random access iterators
  // std::ranges::sort(odd_numbers);  // Doesn't compile

  // OK, odd_number is materialized into a vector
  auto v = to_vector(odd_numbers);
  std::ranges::sort(v);
}

TEST(MaterializeViews, IteratorQualityPreserved) {
  auto vec = std::vector<int>{4, 2, 7, 1, 2, 6, 1, 5};
  auto first_half = vec | std::views::take(vec.size() / 2);
  std::ranges::sort(first_half);
  // vec is now 1, 2, 4, 7, 2, 6, 1, 5

  auto result = std::vector{1, 2, 4, 7, 2, 6, 1, 5};
  ASSERT_EQ(vec, result);
}

#endif // ranges