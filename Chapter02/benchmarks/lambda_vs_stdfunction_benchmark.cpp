#include <benchmark/benchmark.h>
#include <vector>
#include <functional>

//
// Remember to build using optimizations turned on!
// See chapter 3 for an introduction to benchmarking

static void use_lambda(benchmark::State& state) {
  auto lambda = [](int v) { return v * 3; };
  using L = decltype(lambda);
  auto fs = std::vector<L>{};
  fs.resize(10'000'000, lambda);
  auto res = 1;
  for (auto _ : state) {
    for (const auto& f : fs) {
      benchmark::DoNotOptimize(f(res));
    }
  }
}

static void use_std_function(benchmark::State& state) {
  auto lbd = [](int v) { return v * 3; };
  using F = std::function<int(int)>;
  auto fs = std::vector<F>{};
  fs.resize(10'000'000, lbd);
  auto res = 1;
  for (auto _ : state) {
    for (const auto& f : fs) {
      benchmark::DoNotOptimize(f(res));
    }
  }
}

BENCHMARK(use_lambda);
BENCHMARK(use_std_function);
BENCHMARK_MAIN();

