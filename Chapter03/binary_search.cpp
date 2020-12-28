#include <gtest/gtest.h>

#include <cassert>
#include <numeric>
#include <vector>

auto binary_search(const std::vector<int>& a, int key) {
  // Ensure our cast below is safe
  assert(a.size() < std::numeric_limits<int>::max());

  auto low = 0;
  auto high = static_cast<int>(a.size()) - 1;
  while (low <= high) {
    const auto mid = std::midpoint(low, high); // C++20
    if (a[mid] < key) {
      low = mid + 1;
    } else if (a[mid] > key) {
      high = mid - 1;
    } else {
      return true;
    }
  }
  return false;
}

TEST(BinarySearch, FindNumber) {
  auto a = std::vector{1, 2, 3, 4, 5, 6, 7, 8};
  auto found = binary_search(a, 7);
  ASSERT_TRUE(found);
  found = binary_search(a, 4);
  ASSERT_TRUE(found);
  found = binary_search(a, 1);
  ASSERT_TRUE(found);
  found = binary_search(a, 8);
  ASSERT_TRUE(found);
  found = binary_search(a, 10);
  ASSERT_FALSE(found);
  found = binary_search(a, 0);
  ASSERT_FALSE(found);

  found = binary_search({}, 10);
  ASSERT_FALSE(found);
  found = binary_search({1}, 0);
  ASSERT_FALSE(found);
  found = binary_search({1}, 1);
  ASSERT_TRUE(found);
  found = binary_search({1}, 2);
  ASSERT_FALSE(found);
  
}
