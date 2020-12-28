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

auto setup_fixture(size_t n) {
  auto src = std::vector<float>(n);
  std::iota(src.begin(), src.end(), 1.0f); // "src" goes from 1.0 to n
  auto dst = std::vector<float>(src.size());
  auto transform_func = [](float v) {
    auto sum = v;
    for (auto i = 0; i < 500; ++i) {
      sum += (i * i * i * sum);
    }
    return sum;
  };
  return std::tuple{src, dst, transform_func};
}

void bm_sequential(benchmark::State& state) {
  auto [src, dst, f] = setup_fixture(state.range(0));
  for (auto _ : state) {
    std::transform(src.begin(), src.end(), dst.begin(), f);
  }
}

void bm_parallel(benchmark::State& state) {
  auto [src, dst, f] = setup_fixture(state.range(0));
  for (auto _ : state) {
    par_transform_naive(src.begin(), src.end(), dst.begin(), f);
  }
}

void CustomArguments(benchmark::internal::Benchmark* b) {
  b->Arg(50)->Arg(5000)->Arg(50'000)->Arg(100'000)->Arg(10'000'000)
      ->MeasureProcessCPUTime() 
      ->UseRealTime()        
      ->Unit(benchmark::kMillisecond);
}

} // namespace

BENCHMARK(bm_sequential)->Apply(CustomArguments);
BENCHMARK(bm_parallel)->Apply(CustomArguments);

BENCHMARK_MAIN();
