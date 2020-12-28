#include "transform.h"

#include <algorithm>
#include <cassert>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

TEST(Transform, Sequential) {

  const auto src = std::vector{1, 2, 3, 4};
  auto dst = std::vector<int>(src.size());
  auto f = [](int x) { return x * x; };
  ::transform(src.begin(), src.end(), dst.begin(), f);
  for (size_t i = 0; i < dst.size(); ++i) {
    ASSERT_TRUE(dst.at(i) == f(src.at(i)));
  }
}

TEST(Transform, NaiveParallel) {
  const auto n = 1'000'000ul;
  auto src = std::vector<int>(n);
  std::iota(src.begin(), src.end(), 0);
  auto dst = std::vector<int>(src.size());
  auto f = [](int x) { return x * x; };
  par_transform_naive(src.begin(), src.end(), dst.begin(), f);

  for (size_t i = 0; i < dst.size(); ++i) {
    ASSERT_TRUE(dst.at(i) == f(src.at(i)));
  }
}

TEST(Transform, DivideAndConquer) {
  const auto n = 1'000'000ul;
  const auto chunk_sz = 1'000ul;

  auto src = std::vector<int>(n);
  std::iota(src.begin(), src.end(), 0);
  auto dst = std::vector<int>(src.size());
  auto f = [](int x) { return x * x; };

  par_transform(src.begin(), src.end(), dst.begin(), f, chunk_sz);
  for (size_t i = 0; i < dst.size(); ++i) {
    ASSERT_TRUE(dst.at(i) == f(src.at(i)));
  }
}
