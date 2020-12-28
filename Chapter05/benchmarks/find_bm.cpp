#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <numeric>

namespace {

auto create_ints(size_t n) {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 0);
  return v;
}

template <typename It, typename Value>
auto find_slow(It first, It last, const Value& value) {
  for (auto it = first; it != last; ++it)
    if (*it == value)
      return it;
  return last;
}

template <typename It, typename Value>
auto find_fast(It first, It last, const Value& value) {
  // Main loop unrolled into chunks of four
  auto num_trips = (last - first) / 4;
  for (auto trip_count = num_trips; trip_count > 0; --trip_count) {
    if (*first == value) {
      return first;
    }
    ++first;
    if (*first == value) {
      return first;
    }
    ++first;
    if (*first == value) {
      return first;
    }
    ++first;
    if (*first == value) {
      return first;
    }
    ++first;
  }
  // Handle the remaining elements
  switch (last - first) {
  case 3:
    if (*first == value) {
      return first;
    }
    ++first;
    [[fallthrough]];
  case 2:
    if (*first == value) {
      return first;
    }
    ++first;
    [[fallthrough]];
  case 1:
    if (*first == value) {
      return first;
    }
    ++first;
    [[fallthrough]];
  case 0:
  default:
    return last;
  }
}

void bm_find_fast(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  auto element = r.back();
  for (auto _ : state) {
    auto it = find_fast(r.begin(), r.end(), element);
    benchmark::DoNotOptimize(it);
  }
}

void bm_find_slow(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  auto element = r.back();
  for (auto _ : state) {
    auto it = find_slow(r.begin(), r.end(), element);
    benchmark::DoNotOptimize(it);
  }
}

} // namespace

BENCHMARK(bm_find_fast)->Arg(10'000'000)->Unit(benchmark::kMillisecond);
BENCHMARK(bm_find_slow)->Arg(10'000'000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
