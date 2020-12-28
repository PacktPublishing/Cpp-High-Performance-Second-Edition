 // Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <vector>

void print(auto&& r) {
  std::ranges::for_each(r, [](auto&& i) { std::cout << i << ' '; });
}

TEST(EverydayProblems, Transform) {
  auto in = std::vector<int>{1, 2, 3, 4};
  auto out = std::vector<int>(in.size());
  auto lambda = [](auto&& i) { return i * i; };
  std::ranges::transform(in, out.begin(), lambda);
  print(out);
  // Output: 1 4 9 16

  ASSERT_EQ(out.front(), 1);
  ASSERT_EQ(out.back(), 16);
}

//
// Generating elements
//

TEST(EverydayProblems, Fill) {

  std::vector<int> v(4);
  std::ranges::fill(v, -1);
  print(v);
  // Output: -1 -1 -1 -1

  ASSERT_EQ(v.front(), -1);
  ASSERT_EQ(v.back(), -1);
}

TEST(EverydayProblems, Generate) {

  std::vector<int> v(4);
  std::ranges::generate(v, std::rand);
  print(v);

  ASSERT_EQ(v.size(), 4);
}

TEST(EverydayProblems, Iota) {
  auto v = std::vector<int>(6);
  std::iota(v.begin(), v.end(), 0);
  print(v); // Output: 0 1 2 3 4 5 

  ASSERT_EQ(v.front(), 0);
  ASSERT_EQ(v.back(), 5);
}

//
// Sorting
//

TEST(EverydayProblems, Sort) {
  auto v = std::vector<int>{4, 3, 2, 3, 6};
  std::ranges::sort(v);
  print(v); // Output: 2 3 3 4 6 

  ASSERT_EQ(v.front(), 2);
  ASSERT_EQ(v.back(), 6);
}

//
// Finding elements
//

TEST(EverydayProblems, Find) {
  auto col = std::list{2, 4, 3, 2, 3, 1};
  auto it = std::ranges::find(col, 2);
  if (it != col.end()) {
    std::cout << *it << '\n';
  }
}

//
// Binary search algorithms, requires sorted ranges
//

TEST(EverydayProblems, BinarySearch) {
  auto v = std::vector<int>{2, 2, 3, 3, 3, 4, 5}; // Sorted!
  bool found = std::ranges::binary_search(v, 3);
  std::cout << std::boolalpha << found;           // Output: true

  ASSERT_TRUE(found);
}

TEST(EverydayProblems, LowerBound) {
  auto v = std::vector<int>{2, 2, 3, 3, 3, 4, 5};
  auto it = std::ranges::lower_bound(v, 3);
  if (it != v.end()) {
    auto index = std::distance(v.begin(), it);
    std::cout << "Position: " << index << '\n';
  }
}

TEST(EverydayProblems, UpperBound) {
  auto v = std::vector<int>{2, 2, 3, 3, 3, 4, 5};
  auto it = std::ranges::upper_bound(v, 3);
  if (it != v.end()) {
    auto index = std::distance(v.begin(), it);
    std::cout << "Position: " << index << '\n';
  }
  // Output: Position: 5
}

TEST(EverydayProblems, EqualRange) {
  auto v = std::vector<int>{2, 2, 3, 3, 3, 4, 5};
  auto subrange = std::ranges::equal_range(v, 3);
  if (subrange.begin() != subrange.end()) {
    auto pos1 = std::distance(v.begin(), subrange.begin());
    auto pos2 = std::distance(v.begin(), subrange.end());
    std::cout << pos1 << " " << pos2 << '\n';
  }
}

//
// Testing conditions
//

TEST(EverydayProblems, NoneOf) {

  const auto v = std::vector<int>{3, 2, 2, 1, 0, 2, 1};
  auto is_negative = [](int i) { return i < 0; };

  if (std::ranges::none_of(v, is_negative)) {
    std::cout << "Contains only natural numbers\n";
  }
}

TEST(EverydayProblems, AllOf) {

  const auto v = std::vector<int>{3, 2, 2, 1, 0, 2, 1};
  auto is_negative = [](int i) { return i < 0; };

  if (std::ranges::all_of(v, is_negative)) {
    std::cout << "Contains only negative numbers\n";
  }
}

TEST(EverydayProblems, AnyOf) {

  const auto v = std::vector{3, 2, 2, 1, 0, 2, 1};
  auto is_negative = [](int i) { return i < 0; };

  if (std::ranges::any_of(v, is_negative)) {
    std::cout << "Contains at least one negative number\n";
  }
}

//
// Counting elements
//

TEST(EverydayProblems, Count) {
  auto v = std::list{3, 3, 2, 1, 3, 1, 3};
  int n = std::ranges::count(v, 3);
  std::cout << n; // Output: 4
}

TEST(EverydayProblems, CountUsingEqualRangeAndSize) {
  auto v = std::vector{0, 2, 2, 3, 3, 4, 5};
  assert(std::ranges::is_sorted(v)); // O(n), but not called in release
  auto r = std::ranges::equal_range(v, 3);
  int n = std::ranges::size(r);
  std::cout << n; // Output: 2
}

//
// Min and Max values
//

namespace {

const auto y_max = 100;
const auto y_min = 10;

auto some_func() { return 200; }

} // namespace

TEST(EverydayProblems, MinUsingIfStatment) {
  // Avoid this pattern, use std::min() instead
  auto y = some_func();
  if (y > y_max) {
    y = y_max;
  }
  ASSERT_LE(y, y_max);
}

TEST(EverydayProblems, MinAlgorithm) {

  const auto y = std::min(some_func(), y_max);

  ASSERT_LE(y, y_max);
}

TEST(EverydayProblems, ClampUsingMinMax) {

  const auto y = std::max(std::min(some_func(), y_max), y_min);

  ASSERT_LE(y, y_max);
  ASSERT_GE(y, y_min);
}

TEST(EverydayProblems, ClampAlgorithm) {

  const auto y = std::clamp(some_func(), y_min, y_max);

  ASSERT_LE(y, y_max);
  ASSERT_GE(y, y_min);
}

TEST(EverydayProblems, MinMax) {

  auto v = std::vector<int>{4, 2, 1, 7, 3, 1, 5};
  auto [min, max] = std::ranges::minmax(v);
  std::cout << min << " " << max;            // Output: 1 7

  ASSERT_EQ(min, 1);
  ASSERT_EQ(max, 7);
}

TEST(EverydayProblems, MinElement) {
  auto v = std::vector<int>{4, 2, 7, 1, 1, 3};
  auto it = std::ranges::min_element(v);
  std::cout << std::distance(v.begin(), it); // Output: 3
}

#endif // ranges