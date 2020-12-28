#include "../copy_if.h"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <cassert>
#include <future>
#include <numeric>
#include <tuple>
#include <vector>

namespace {

// Test 1
// Naive vs std::transform()
//

// Inexpensive predicate
auto is_odd = [](unsigned v) { return (v % 2) == 1; };

// Expensive predicate
auto is_prime = [](unsigned v) -> bool {
  if (v < 2)
    return false;
  if (v == 2)
    return true;
  if (v % 2 == 0)
    return false;
  for (auto i = 3u; (i * i) <= v; i += 2) {
    if ((v % i) == 0) {
      return false;
    }
  }
  return true;
};

auto setup_fixture(int n) {
  auto src = std::vector<unsigned>(n);
  std::iota(src.begin(), src.end(), 1u); // "src" goes from 1.0 to n
  auto dst = std::vector<unsigned>(src.size());
  return std::tuple{src, dst};
}

template <typename Pred>
void bm_std_copy_if(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end = std::copy_if(src.begin(), src.end(), dst.begin(), pred);
    benchmark::DoNotOptimize(new_end);
  }
}

template <class Pred>
void bm_par_copy_if_sync(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end =
        par_copy_if_sync(src.begin(), src.end(), dst.begin(), pred, 100'000);
    benchmark::DoNotOptimize(new_end);
  }
}

template <class Pred>
void bm_par_copy_if_split(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end =
        par_copy_if_split(src.begin(), src.end(), dst.begin(), pred, 100'000);
    benchmark::DoNotOptimize(new_end);
  }
}

void CustomArguments(benchmark::internal::Benchmark* b) {
  b->MeasureProcessCPUTime()->UseRealTime()->Unit(benchmark::kMillisecond);
}

} // namespace

BENCHMARK_TEMPLATE(bm_std_copy_if, decltype(is_odd))->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_split, decltype(is_odd))->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_sync, decltype(is_odd))->Apply(CustomArguments);

BENCHMARK_TEMPLATE(bm_std_copy_if, decltype(is_prime))->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_split, decltype(is_prime))->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_sync, decltype(is_prime))->Apply(CustomArguments);

BENCHMARK_MAIN();