#include <gtest/gtest.h>
#include <iostream>
#include <vector>

void insertion_sort(std::vector<int>& a) {
  for (size_t i = 1; i < a.size(); ++i) {
    auto j = i;
    while (j > 0 && a[j - 1] > a[j]) {
      std::swap(a[j], a[j - 1]);
      --j;
    }
  }
}

TEST(InsertionSort, SortNumbers) {
  auto a = std::vector<int>{4, 2, 3, 1, 8, 5, 7, 6};

  insertion_sort(a);

  for (auto i : a) {
    std::cout << i << '\n';
  }

  for (size_t i = 1; i < a.size(); ++i) {
    ASSERT_TRUE(a[i - 1] < a[i]);
  }
}
