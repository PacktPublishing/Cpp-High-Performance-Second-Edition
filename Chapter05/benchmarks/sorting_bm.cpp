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

void bm_sort(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  auto rd = std::random_device{};
  auto g = std::mt19937{rd()};
  for (auto _ : state) {
    state.PauseTiming();
    std::shuffle(std::begin(r), std::end(r), g);
    state.ResumeTiming();
    
    std::sort(std::begin(r), std::end(r));
  }
}

void bm_median(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  auto rd = std::random_device{};
  auto g = std::mt19937{rd()};
  for (auto _ : state) {
    state.PauseTiming();
    std::shuffle(r.begin(), r.end(), g);
    state.ResumeTiming();

    auto middle = r.begin() + r.size() / 2;
    std::nth_element(r.begin(), middle, r.end());
    auto median = *middle;
    benchmark::DoNotOptimize(median);
  }
}

void bm_partial_sort(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  auto rd = std::random_device{};
  auto g = std::mt19937{rd()};
  for (auto _ : state) {
    state.PauseTiming();
    std::shuffle(std::begin(r), std::end(r), g);
    state.ResumeTiming();

    std::partial_sort(r.begin(), r.begin() + n / 10, r.end());
  }
}

} // namespace

BENCHMARK(bm_sort)->Arg(10'000'000)->Unit(benchmark::kMillisecond);
BENCHMARK(bm_median)->Arg(10'000'000)->Unit(benchmark::kMillisecond);
BENCHMARK(bm_partial_sort)->Arg(10'000'000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();