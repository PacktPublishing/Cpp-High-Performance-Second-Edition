
#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <version>


#if defined(__cpp_lib_ranges)
#include <ranges>
TEST(PipeOperator, RangesExample) {
  const auto r = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
  auto odd_positive_numbers =
      r | std::views::filter([](auto v) { return v > 0; }) |
      std::views::filter([](auto v) { return (v % 2) == 1; });

  auto it = std::begin(odd_positive_numbers);
  ASSERT_EQ(*it++, 1);
  ASSERT_EQ(*it++, 3);
  ASSERT_EQ(*it++, 5);
  ASSERT_EQ(it, std::end(odd_positive_numbers));
}
#endif // ranges


template <typename T> struct ContainsProxy { const T& value_; };

template <typename Range, typename T>
auto operator|(const Range& r, const ContainsProxy<T>& proxy) {
  const auto& v = proxy.value_;
  return std::find(r.begin(), r.end(), v) != r.end();
}

template <typename T> auto contains(const T& v) { return ContainsProxy<T>{v}; }

TEST(PipeOperator, Numbers) {
  auto numbers = std::vector<int>{1, 3, 5, 7, 9};
  auto seven = 7;
  auto proxy = ContainsProxy<decltype(seven)>{seven};
  bool has_seven = numbers | proxy;
  ASSERT_TRUE(has_seven);
}

TEST(PipeOperator, Strings) {
  auto penguins = std::vector<std::string>{"Ping", "Roy", "Silo"};
  bool has_silo = penguins | contains("Silo");
  ASSERT_TRUE(has_silo);
}

