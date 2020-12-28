#include <gtest/gtest.h>

#include "lin_value.h"

#include <sstream>
#include <vector>

namespace {

template <typename T>
struct LinSpace {
  LinSpace(T start, T stop, std::size_t n)
      : begin_{start, stop, 0, n}, end_{n} {}

  struct Iterator {
    using difference_type = void;
    using value_type = T;
    using reference = T;
    using pointer = T*;
    using iterator_category = std::forward_iterator_tag;
    void operator++() { ++i_; }
    T operator*() { return lin_value(start_, stop_, i_, n_); }
    bool operator==(std::size_t i) const { return i_ == i; }
    T start_{};
    T stop_{};
    std::size_t i_{};
    std::size_t n_{};
  };
  auto begin() { return begin_; }
  auto end() { return end_; }

private:
  Iterator begin_{};
  std::size_t end_{};
};

template <typename T>
auto lin_space(T start, T stop, size_t n) {
  return LinSpace{start, stop, n};
}

} // namespace

TEST(LinSpaceIterator, Example) {
  std::ostringstream os;
  for (auto v : lin_space(2.0, 3.0, 5)) {
    os << v << ", ";
  }
  ASSERT_EQ("2, 2.25, 2.5, 2.75, 3, ", os.str());
}
