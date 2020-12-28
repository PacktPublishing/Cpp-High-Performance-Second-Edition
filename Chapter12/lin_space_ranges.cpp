#include <gtest/gtest.h>

#include "lin_value.h"

#include <sstream>
#include <version>

#if defined(__cpp_lib_ranges)
#include <ranges>

namespace {

template <typename T>
auto lin_space(T start, T stop, std::size_t n) {
  return std::views::iota(0ul, n) |
    std::views::transform([=](auto i) {
	   return lin_value(start, stop, i, n);
    });
}

} // namespace

TEST(LinSpaceRanges, Example) {

  for (auto v : lin_space(2.0, 3.0, 5)) {
    std::cout << v << ", ";
  }
  // Prints: 2, 2.25, 2.5, 2.75, 3,

  std::ostringstream os;
  for (auto v : lin_space(2.0, 3.0, 5)) {
    os << v << ", ";
  }
  ASSERT_EQ("2, 2.25, 2.5, 2.75, 3, ", os.str());
}

#endif // __cpp_lib_ranges
