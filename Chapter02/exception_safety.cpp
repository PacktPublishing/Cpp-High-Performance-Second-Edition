#include <cassert>
#include <gtest/gtest.h>

namespace {

struct Number {

  // We assume that this class may throw on construction / assignment
  // Number(const Number& other) {
  //     ...
  //     throw ...
  // }
  // auto& operator=(const Number& other) {
  //     ...
  //     throw ...
  // }
  // ...

  // Use consistent comparison (C++20)
  // auto operator<=>(const Number&) const = default; // [ Requires C++20 ]
  // Fallback if not supporting operator<=>
  bool operator==(const Number& rhs) const { return i_ == rhs.i_; }
  bool operator!=(const Number& rhs) const { return !(*this == rhs); }
  int i_{};
};

class Widget {
public:
  Widget(const Number& x, const Number& y) : x_{x}, y_{y} {
    assert(is_valid());
  }
  void update(const Number& a, const Number& b);
  bool is_valid() const { // Class invariant
    return x_ != y_;
  }

private:
  Number x_{};
  Number y_{};
};

// An exception safe implementation using the copy-and-swap idiom
void Widget::update(const Number& x, const Number& y) {
  assert(x != y); // Precondition
  assert(is_valid());
  // Perform operations which may throw without modifying
  // the state of this Widget
  auto x_tmp = x; // May throw, but Widget is still valid
  auto y_tmp = y; // May throw, but Widget is still valid
  // No exceptions thrown, we can safely modify
  // the state of this by non-throwing functions
  std::swap(x_tmp, x_);
  std::swap(y_tmp, y_);
  assert(is_valid()); // Postcondition
}

TEST(ExceptionSafety, CopyAndSwap) {
  auto w = Widget{{1}, {2}};
  w.update({2}, {3});
  ASSERT_TRUE(w.is_valid());
}

} // namespace