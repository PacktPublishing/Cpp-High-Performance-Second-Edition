#include <version>
#if defined(__cpp_lib_execution) && defined(__cpp_lib_parallel_algorithm)

#include <gtest/gtest.h>

#include "copy_if.h"

#include <cassert>
#include <execution>
#include <vector>

TEST(CopyIf, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto new_end =
      ::copy_if(numbers.begin(), numbers.end(), odd_numbers.begin(), is_odd);
  odd_numbers.erase(new_end, odd_numbers.end());

  ASSERT_EQ(odd_numbers, (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15}));
}

TEST(CopyIfSplitIntoTwoParts, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto end = par_copy_if_split(numbers.begin(), numbers.end(),
                               odd_numbers.begin(), is_odd, 4);
  odd_numbers.erase(end, odd_numbers.end());
  ASSERT_EQ(odd_numbers, (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15}));
}

TEST(CopyIfSyncronizedWritePosition, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto odd_numbers = std::vector<int>(numbers.size(), -1);
  auto is_odd = [](int v) { return (v % 2) == 1; };

  auto end = par_copy_if_sync(numbers.begin(), numbers.end(),
                              odd_numbers.begin(), is_odd, 4);
  odd_numbers.erase(end, odd_numbers.end());
  ASSERT_TRUE(std::all_of(odd_numbers.begin(), odd_numbers.end(), is_odd));

  std::sort(odd_numbers.begin(), odd_numbers.end());
  ASSERT_EQ(odd_numbers, (std::vector<int>{1, 3, 5, 7, 9, 11, 13, 15}));
}

#endif // par execution