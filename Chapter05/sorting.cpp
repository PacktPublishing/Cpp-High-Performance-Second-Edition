// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>


TEST(Sorting, SortRanges) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  std::ranges::sort(v);
  ASSERT_TRUE(std::ranges::is_sorted(v));
}

TEST(Sorting, NthElement) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  auto it = v.begin() + v.size() / 2;

  std::nth_element(v.begin(), it, v.end());

  // it is the only sorted element
  ASSERT_EQ(*it, 4);
  // Max element in the left subrange
  auto left = *std::max_element(v.begin(), it);
  // Min element in the right subrange
  auto right = *std::min_element(it + 1, v.end());
  ASSERT_TRUE(left <= right);
}

TEST(Sorting, PartialSort) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  auto it = v.begin() + v.size() / 2;

  std::partial_sort(v.begin(), it, v.end());

  ASSERT_TRUE(std::is_sorted(v.begin(), it));
}


//
// Additional use cases
//

TEST(Sorting, SortSpecificPartOfRange) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  auto it = v.begin() + v.size() / 2;
  auto left = it - 1;
  auto right = it + 2;
  std::nth_element(v.begin(), left, v.end());
  std::partial_sort(left, right, v.end());

  ASSERT_TRUE(std::is_sorted(left, right));
}

TEST(Sorting, SortBackOfRange) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  auto it = v.begin() + v.size() / 2;
  std::nth_element(v.begin(), it, v.end());
  std::sort(it, v.end());

  ASSERT_TRUE(std::is_sorted(it, v.end()));
}

TEST(Sorting, SortFrontAndBack) {
  auto v = std::vector{6, 3, 2, 7, 4, 1, 5};
  auto it = v.begin() + v.size() / 2;
  auto left = it - 1;
  auto right = it + 2;
  std::nth_element(v.begin(), right, v.end());
  std::partial_sort(v.begin(), left, right);
  std::sort(right, v.end());

  ASSERT_TRUE(std::is_sorted(v.begin(), left));
  ASSERT_TRUE(std::is_sorted(right, v.end()));
}

#endif // ranges