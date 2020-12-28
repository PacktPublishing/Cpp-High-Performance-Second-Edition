// Requires C++20
#include <version>
#if defined(__cpp_concepts) && defined(__cpp_lib_concepts)

#include <gtest/gtest.h>

#include <cmath>
#include <concepts>
#include <type_traits>

namespace {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept Point = requires(T p) {
  requires std::is_same_v<decltype(p.x()), decltype(p.y())>;
  requires Arithmetic<decltype(p.x())>;
};

// "auto" is not supported as function paramter in visual studio at the time of writing this
#ifndef _MSC_VER
std::floating_point auto dist(Point auto p1, Point auto p2) {
  auto a = p1.x() - p2.x();
  auto b = p1.y() - p2.y();
  return std::sqrt(a * a + b * b);
}
#else
template <Point P>
std::floating_point auto dist(P p1, P p2) {
  auto a = p1.x() - p2.x();
  auto b = p1.y() - p2.y();
  return std::sqrt(a * a + b * b);
}
#endif



template <Arithmetic T> // T is now constrained!
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

} // namespace

TEST(Point2DConcepts, Dist) {
  auto p1 = Point2D{2, 2};
  auto p2 = Point2D{6, 5};
  auto d = dist(p1, p2);

  std::cout << d;
  ASSERT_DOUBLE_EQ(d, 5.0);

  // Trying to instatiate Point2D with
  // unrelevant types is no longer possible
  // auto p = Point2D{"2.0", "2.0"};
}

#endif // concepts