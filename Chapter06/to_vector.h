#pragma once

#include <algorithm>
#include <ranges>
#include <vector>

// Materialize the range r into a std::vector
// See also https://timur.audio/how-to-make-a-container-from-a-c20-range
auto to_vector(auto&& r) {
  std::vector<std::ranges::range_value_t<decltype(r)>> v;
  if constexpr (std::ranges::sized_range<decltype(r)>) {
    v.reserve(std::ranges::size(r));
  }
  std::ranges::copy(r, std::back_inserter(v));
  return v;
}