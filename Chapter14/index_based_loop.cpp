#include <version>
#if defined(__cpp_lib_execution) && \
    defined(__cpp_lib_parallel_algorithm) && \
    defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <ranges>


namespace {

template <typename Policy, typename Index, typename F>
auto parallel_for(Policy&& p, Index first, Index last, F f) {
  auto r = std::views::iota(first, last + 1);
  std::for_each(p, r.begin(), r.end(), std::move(f));
}

} // namespace

TEST(IndexBasedLoop, ParallelFor) {
  auto v = std::vector<std::string>{"A", "B", "C"};
  parallel_for(std::execution::par, size_t{0}, v.size() - 1,
               [&v](size_t i) { v[i] += std::to_string(i + 1); });

  auto os = std::ostringstream{};
  for (auto&& s : v) {
    os << s << ' ';
  }
  auto s = os.str();
  ASSERT_EQ("A1 B2 C3 ", s);
}

#endif // par execution