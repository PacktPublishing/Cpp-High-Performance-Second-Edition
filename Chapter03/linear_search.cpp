#include <cassert>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

auto linear_search(const std::vector<int>& vals, int key) {
  for (const auto& v : vals) {
    if (v == key) {
      return true;
    }
  }
  return false;
}

TEST(LinearSearch, FindNumber) {
  auto a = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};
  auto found = linear_search(a, 7);

  ASSERT_TRUE(found);
}
