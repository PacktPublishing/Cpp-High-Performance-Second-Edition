#if _MSC_VER
  #pragma warning(disable: 4834) // discarding return value of function with 'nodiscard' attribute
#endif

#include <cassert>
#include <optional>
#include <vector>

#include <gtest/gtest.h>

namespace {

// Prerequisite
struct Point {
  /*...*/
};
struct Line {
  /*...*/
};

// Dummy functions
auto lines_are_parallel(Line /*a*/, Line /*b*/) -> bool { return false; }
auto compute_intersection(Line /*a*/, Line /*b*/) -> Point { return Point{}; }
auto set_magic_point(Point) { /*...*/ }

auto get_intersection(const Line& a, const Line& b) -> std::optional<Point> {
  if (lines_are_parallel(a, b))
    return std::optional{compute_intersection(a, b)};
  else
    return {};
}

} // namespace

TEST(Optional, Intersection) {
  // Get optional intersection
  auto intersection = get_intersection(Line{}, Line{});
  if (intersection.has_value()) {
    set_magic_point(*intersection);
  }
  ASSERT_FALSE(intersection.has_value());
  // std::optional throws an exception if empty
  EXPECT_THROW(intersection.value(), std::bad_optional_access);
}

namespace {

struct Hat { /*...*/
};

class Head {
public:
  Head() { assert(!hat_); } // hat_ is empty by default
  auto set_hat(const Hat& h) { hat_ = h; }
  auto has_hat() const { return hat_.has_value(); }
  auto& get_hat() const {
    assert(hat_.has_value());
    return *hat_;
  }
  auto remove_hat() {
    hat_ = {}; // Hat is cleared by assigning to {}
  }

private:
  std::optional<Hat> hat_;
};

} // namespace

TEST(Optional, Head) {
  auto a = Head{};
  auto b = a;
  ASSERT_FALSE(a.has_hat());
  ASSERT_FALSE(b.has_hat());
  a.set_hat(Hat{});
  ASSERT_TRUE(a.has_hat());
  ASSERT_FALSE(b.has_hat());
}

TEST(Optional, Sort) {
  auto c = std::vector<std::optional<int>>{{3}, {}, {1}, {}, {2}};
  std::sort(c.begin(), c.end());
  const auto sorted = std::vector<std::optional<int>>{{}, {}, {1}, {2}, {3}};
  ASSERT_EQ(sorted, c);
}
