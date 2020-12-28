#include "../string_concat_proxy.h"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
// #include <ranges> // Requires C++20
#include <string>
#include <vector>

namespace {

template <typename T> 
auto create_strings(int n, size_t length) {
  auto re = std::default_random_engine{std::random_device{}()};
  auto random_string = [&] {
    auto d = std::uniform_int_distribution<>{'A', 'Z'};
    auto random_char = [&] { return static_cast<char>(d(re)); };
    auto s = std::string(length, '\0');
    // std::ranges::generate(s, random_char); // Requires C++20
    std::generate(std::begin(s), std::end(s), random_char);
    return s;
  };
  auto v = std::vector<T>(n);
  // std::ranges::generate(v, random_string);
  std::generate(std::begin(v), std::end(v), random_string);
  return v;
}

template <typename T> void bm_string_compare(benchmark::State& state) {
  const auto n = 10'000;
  const auto length = state.range(0);
  const auto a = create_strings<T>(n, length);
  const auto b = create_strings<T>(n, length);
  const auto c = create_strings<T>(n, length * 2);
  for (auto _ : state) {
    for (auto i = 0; i < n; ++i) {
      auto is_equal = a[i] + b[i] == c[i];
      benchmark::DoNotOptimize(is_equal);
    }
  }
}

} // namespace

BENCHMARK_TEMPLATE(bm_string_compare, std::string)->Arg(50)->Arg(10)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(bm_string_compare, String)->Arg(50)->Arg(10)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();