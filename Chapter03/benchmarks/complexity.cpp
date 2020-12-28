#include <benchmark/benchmark.h>
#include <iostream>
#include <numeric>
#include <string>

auto linear_search(const std::vector<int>& vec, int v) -> bool {
  for (auto&& i : vec) {
    if (i == v) {
      return true;
    }
  }
  return false;
}

auto binary_search(const std::vector<int>& a, int key) {
  if (a.empty()) {
    return false;
  }
  auto low = size_t{0};
  auto high = a.size() - 1;
  while (low <= high) {
    const auto mid = std::midpoint(low, high);
    if (a[mid] < key) {
      low = mid + 1;
    } else if (a[mid] > key) {
      high = mid - 1;
    } else {
      return true;
    }
  }
  return false;
}

auto gen_vec(int s) {
  std::vector<int> v(s);
  for (int i = 0; i < s; ++i) {
    v[i] = i;
  }
  return v;
}

static void bm_linear_search(benchmark::State& state) {
  auto n = static_cast<int>(state.range(0));
  auto v = gen_vec(n);
  for (auto _ : state) {
    benchmark::DoNotOptimize(linear_search(v, n));
  }
  state.SetComplexityN(n);
}

static void bm_binary_search(benchmark::State& state) {
  auto n = static_cast<int>(state.range(0));
  auto v = gen_vec(n);
  for (auto _ : state) {
    benchmark::DoNotOptimize(binary_search(v, n));
  }
  state.SetComplexityN(n);
}

BENCHMARK(bm_linear_search)->RangeMultiplier(2)->Range(64, 4096)->Complexity();
BENCHMARK(bm_binary_search)->RangeMultiplier(2)->Range(64, 4096)->Complexity();
BENCHMARK_MAIN();
