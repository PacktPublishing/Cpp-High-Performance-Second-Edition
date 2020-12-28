// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

TEST(UnderstandingViews, LazyEvaluation) {
  auto numbers = std::vector{1, 2, 3, 4};
  auto square = [](auto v) { return v * v; };

  auto squared_view = std::views::transform(numbers, square);
  for (auto s : squared_view) { // The square lambda is invoked here
    std::cout << s << " ";
  }
  // Output: 1 4 9 16
}

TEST(UnderstandingViews, FilterViews) {
  auto v = std::vector{4, 5, 6, 7, 6, 5, 4};
  auto odd_view = std::views::filter(v, [](auto i) { return (i % 2) == 1; });
  for (auto odd_number : odd_view) {
    std::cout << odd_number << " ";
  }
  // Output: 5 7 5
}

TEST(UnderstandingViews, FlattenAndJoin) {
  auto list_of_lists =
      std::vector<std::vector<int>>{{1, 2}, {3, 4, 5}, {5}, {4, 3, 2, 1}};
  auto flattened_view = std::views::join(list_of_lists);
  for (auto v : flattened_view)
    std::cout << v << " ";
  // Output: 1 2 3 4 5 5 4 3 2 1

  auto max_value = *std::ranges::max_element(flattened_view);
  // max_value is 5
  ASSERT_EQ(max_value, 5);
}

#endif // ranges