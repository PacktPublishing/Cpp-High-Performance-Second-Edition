// Requires C++20
// Compile using gcc 10: g++ -std=c++2a -O3 -lpthread -Wall
#include <version>
#if defined(__cpp_lib_atomic_ref) && defined(__cpp_lib_jthread)

#include <gtest/gtest.h>

#include <atomic>
#include <iostream>
#include <random>
#include <thread>

namespace {

auto random_int(int min, int max) {
  // One engine instance per thread
  static thread_local auto engine =
      std::default_random_engine{std::random_device{}()};

  auto dist = std::uniform_int_distribution<>{min, max};
  return dist(engine);
}

struct Stats {
  int heads{};
  int tails{};
};

std::ostream& operator<<(std::ostream& os, const Stats& s) {
  os << "heads: " << s.heads << ", tails: " << s.tails;
  return os;
}

void flip_coin(std::size_t n, Stats& outcomes) {
  auto flip = [&outcomes](auto n) {
    auto heads = std::atomic_ref<int>{outcomes.heads};
    auto tails = std::atomic_ref<int>{outcomes.tails};
    for (auto i = 0u; i < n; ++i) {
      random_int(0, 1) == 0 ? ++heads : ++tails;
    }
  };
  auto t1 = std::jthread{flip, n / 2};       // First half
  auto t2 = std::jthread{flip, n - (n / 2)}; // The rest
}

TEST(Atomics, AtomicReferences) {
  auto stats = Stats{};
  flip_coin(5000, stats); // Flip 5000 times
  std::cout << stats << '\n';
  ASSERT_EQ(5000, (stats.tails + stats.heads));
}

} // namespace

#endif // atomic ref && jthread