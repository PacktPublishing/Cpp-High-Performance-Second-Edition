#include "../transform.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <cassert>
#include <future>
#include <tuple>
#include <vector>
#include <numeric>

namespace {

// Test 1
// Naive vs std::transform()
//

auto setup_fixture(int n) {
  auto src = std::vector<float>(n);
  std::iota(src.begin(), src.end(), 1.0f); // From 1.0 to n
  auto dst = std::vector<float>(src.size());
  auto transform_function = [](float v) {
    auto sum = v;
    auto n = v / 20'000;             // The larger v is,
    for (auto i = 0; i < n; ++i) { // the more to compute
      sum += (i * i * i * sum);
    }
    return sum;
  };
  return std::tuple{src, dst, transform_function};
}

void bm_parallel(benchmark::State& state) {
  auto [src, dst, f] = setup_fixture(10'000'000);
  auto chunk_sz = state.range(0);
  for (auto _ : state) {
    par_transform(src.begin(), src.end(), dst.begin(), f, chunk_sz);
  }
}

void bm_parallel_naive(benchmark::State& state) {
  auto [src, dst, f] = setup_fixture(10'000'000);
  for (auto _ : state) {
    par_transform_naive(src.begin(), src.end(), dst.begin(), f);
  }
}

void CustomArguments(benchmark::internal::Benchmark* b) {
  b->MeasureProcessCPUTime()->UseRealTime()->Unit(benchmark::kMillisecond);
}

} // namespace

BENCHMARK(bm_parallel)->Apply(CustomArguments)
     ->RangeMultiplier(10)
     ->Range(10000, 10'000'000);
BENCHMARK(bm_parallel_naive)->Apply(CustomArguments);

BENCHMARK_MAIN();
