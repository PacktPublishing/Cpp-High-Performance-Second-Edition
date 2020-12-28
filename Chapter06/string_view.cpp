// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include "to_vector.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstring>
#include <ranges>
#include <span>
#include <string>
#include <vector>

namespace {

auto split(std::string_view s, char delim) {
  const auto to_string = [](auto&& r) -> std::string {
    const auto cv = std::ranges::common_view{r};
    return {cv.begin(), cv.end()};       
  };
  const auto v =
      std::ranges::split_view{s, delim} | std::views::transform(to_string);
  return to_vector(v);
}

} // namespace

TEST(StringView, Split) {

  const char* c_str = "ABC,DEF,GHI";
  const auto v1 = split(c_str, ',');

  const auto s = std::string{"ABC,DEF,GHI"};
  const auto v2 = split(s, ',');
  
  ASSERT_EQ(v1, v2);
}

#endif // ranges