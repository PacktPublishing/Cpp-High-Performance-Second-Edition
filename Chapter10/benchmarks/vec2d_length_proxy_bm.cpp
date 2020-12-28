#include "../vec2d_length_proxy.h"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cmath>
// #include <ranges> // Requires C++20
#include <string>
#include <vector>
#include <iterator>


namespace {


// A non-optimized version for benchmarking
struct Vec2DSlow {
  auto length() const {
    auto squared = x_ * x_ + y_ * y_;
    return std::sqrt(squared);
  }
  float x_, y_;
};

template <typename T> auto min_length(const T& r) -> float {
  assert(!r.empty());
  auto cmp = [](auto&& a, auto&& b) { return a.length() < b.length(); };
  // auto it = std::ranges::min_element(r, cmp);
  auto it = std::min_element(std::begin(r), std::end(r), cmp);
  return it->length();
}

template <typename T> void bm_min_length(benchmark::State& state) {
  auto v = std::vector<T>{};
  std::generate_n(std::back_inserter(v), 1000, [] {
    auto x = static_cast<float>(std::rand());
    auto y = static_cast<float>(std::rand());
    return T{x, y};
  });
  for (auto _ : state) {
    auto res = min_length(v);
    benchmark::DoNotOptimize(res);
  }
}

} // namespace

BENCHMARK_TEMPLATE(bm_min_length, Vec2DSlow);
BENCHMARK_TEMPLATE(bm_min_length, Vec2D);

BENCHMARK_MAIN();
