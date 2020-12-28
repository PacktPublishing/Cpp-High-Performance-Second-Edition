#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <gtest/gtest.h>

#include "generator.h"

#include <limits>

Generator<int> iota(int start) {
  for (int i = start; i < std::numeric_limits<int>::max(); ++i) {
    co_yield i;
  }
}

Generator<int> take_until(Generator<int>& gen, int value) {
  for (auto v : gen) {
    if (v == value) {
      co_return;
    }
    co_yield v;
  }
}

TEST(GeneratorInitialExamlpe, IotaAndTakeUntil) {
  auto i = iota(2);
  auto t = take_until(i, 5);
  for (auto v : t) {          // Pull values
    std::cout << v << ", ";
  }
}
// Prints: 2, 3, 4

#endif // SUPPORTS_COROUTINES