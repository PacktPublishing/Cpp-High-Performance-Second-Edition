
//#include <version>
//#if defined(__cpp_lib_execution) && defined(__cpp_lib_parallel_algorithm)

#include <gtest/gtest.h>

#include <algorithm>
#include <future>
#include <vector>

template <typename It, typename Pred>
auto par_count_if(It first, It last, Pred pred, size_t chunk_sz) {
  auto n = static_cast<size_t>(std::distance(first, last));
  if (n <= chunk_sz)
    return std::count_if(first, last, pred);
  auto middle = std::next(first, n / 2);
  auto fut = std::async(std::launch::async, [=, &pred] {
    return par_count_if(first, middle, pred, chunk_sz);
  });
  auto num = par_count_if(middle, last, pred, chunk_sz);
  return num + fut.get();
}

template <typename It, typename Pred>
auto par_count_if(It first, It last, Pred pred) {
  auto n = static_cast<size_t>(std::distance(first, last));
  auto n_cores = size_t{std::thread::hardware_concurrency()};
  auto chunk_sz = std::max(n / n_cores * 32, size_t{1000});
  return par_count_if(first, last, pred, chunk_sz);
}

TEST(CountIf, OddNumbers) {
  auto numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  auto is_odd = [](int v) { return (v % 2) == 1; };
  auto count = par_count_if(numbers.begin(), numbers.end(), is_odd);
  ASSERT_EQ(numbers.size() / 2, count);
}
