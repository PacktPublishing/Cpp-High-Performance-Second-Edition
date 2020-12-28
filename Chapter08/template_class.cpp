#include <gtest/gtest.h>

// Rectangle can be of any type
template <typename T> 
class Rectangle {
public:
  Rectangle(T x, T y, T w, T h) : x_{x}, y_{y}, w_{w}, h_{h} {}
  auto area() const { return w_ * h_; }
  auto width() const { return w_; }
  auto height() const { return h_; }

private:
  T x_{}, y_{}, w_{}, h_{};
};

template <typename T> 
auto is_square(const Rectangle<T>& r) {
  return r.width() == r.height();
}

TEST(TemplateClass, FloatAndIntSquare) {
  auto r1 = Rectangle<float>{2.0f, 2.0f, 4.0f, 4.0f};
  auto r2 = Rectangle{-2, -2, 4, 4};

  ASSERT_EQ(typeid(Rectangle<float>), typeid(r1));
  ASSERT_EQ(typeid(Rectangle<int>), typeid(r2));
  ASSERT_TRUE(is_square(r1));
  ASSERT_TRUE(is_square(r2));
}
