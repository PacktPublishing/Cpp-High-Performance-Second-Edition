#include <gtest/gtest.h>

#include <cmath>
#include <iostream>

namespace {

template <typename T>
class Point2D {
public:
  Point2D(T x, T y) : x_{x}, y_{y} {}
  auto x() { return x_; }
  auto y() { return y_; }
  // …
private:
  T x_{};
  T y_{};
};

template <typename T>
auto dist(T p1, T p2) {
  auto a = p1.x() - p2.x();
  auto b = p1.y() - p2.y();
  return std::sqrt(a * a + b * b);
};

} // namespace

TEST(Point2DUnconstrained, Dist) {
  auto p1 = Point2D{2, 2};
  auto p2 = Point2D{6, 5};
  auto d = dist(p1, p2);

  std::cout << d;
  ASSERT_DOUBLE_EQ(d, 5.0);

  // Doesn't compile, int doesn't have x() and y() functions
  // We want better error messages when trying to instantiate
  // this function template
  // dist(3, 4);
}

TEST(Point2DUnconstrained, DistWithCharPtr) {
  // We don't want this to compile!
  auto from = Point2D{"2.0", "2.0"}; // Ouch!
  auto to = Point2D{"6.0", "5.0"};   // Point2D<const char*>
  auto d = dist(from, to);
  std::cout << d;          // Prints nonsense
}