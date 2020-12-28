#include <cassert>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

struct Point {
  int x{};
  int y{};
};

auto linear_search(const std::vector<Point>& a, const Point& key) {
  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i].x == key.x && a[i].y == key.y) {
      return true;
    }
  }
  return false;
}

TEST(LinearSearchPoint, FindPoint) {
  auto a = std::vector<Point>{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
  auto found = linear_search(a, {7, 8});

  ASSERT_TRUE(found);
}
