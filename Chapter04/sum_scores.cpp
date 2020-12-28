#include <iostream>
#include <array>
#include <vector>
#include <gtest/gtest.h>
#include "scoped_timer.h"

//
// This example demonstrates that traversing a contiguous array
// of small objects is faster than traversing an array of big objects.
//

struct SmallObject {
  std::array<char, 4> data_{};
  int score_{};
  SmallObject() : score_{std::rand()} {}
};

struct BigObject {
  std::array<char, 256> data_{};
  int score_{};
  BigObject() : score_{std::rand()} {}
};

template <class T>
auto sum_scores(const std::vector<T>& objects) {
  ScopedTimer t{"sum_scores"};
  auto sum = 0;
  for (const auto& obj : objects) {
    sum += obj.score_;
  }
  return sum;
}

TEST(SumScores, CompareProcessingTime) {
  std::cout << "sizeof(SmallObject): " << sizeof(SmallObject) << " bytes" << '\n';
  std::cout << "sizeof(BigObject): " << sizeof(BigObject) << " bytes" << '\n';

  auto num_objects = 1'000'000;
  auto small_objects = std::vector<SmallObject>(num_objects);
  auto big_objects = std::vector<BigObject>(num_objects);

  std::cout << "+++ sum_scores using SmallObject +++" << '\n';
  auto small_sum = 0ul;
  small_sum += sum_scores(small_objects);
  small_sum += sum_scores(small_objects);
  small_sum += sum_scores(small_objects);
  small_sum += sum_scores(small_objects);

  std::cout << "+++ sum_scores using BigObject +++" << '\n';
  auto big_sum = 0ul;
  big_sum += sum_scores(big_objects);
  big_sum += sum_scores(big_objects);
  big_sum += sum_scores(big_objects);
  big_sum += sum_scores(big_objects);

  std::cout << "small sum: " << small_sum << '\n';
  std::cout << "big sum: " << big_sum << '\n';
  std::cout << "total sum: " << small_sum + big_sum << '\n';
}
