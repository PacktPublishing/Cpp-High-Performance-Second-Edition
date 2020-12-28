// Compile using clang (trunk) with:
// -std=c++2a -Wall -stdlib=libc++ -O3 -lpthread
#include <version>
#if defined(__cpp_lib_latch)

#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>

void prefault_stack() {
  constexpr auto stack_size =
      500u * 1024u;                       // We don't know the size of the stack
  volatile unsigned char mem[stack_size]; // Make volatile to avoid optimization
  std::fill(std::begin(mem), std::end(mem), 0);
}

void do_work() { std::cout << "Thread is working\n"; }

TEST(Latches, PrefaultStack) {
  constexpr auto n_threads = 2;
  auto initialized = std::latch{n_threads};
  auto threads = std::vector<std::thread>{};
  for (auto i = 0; i < n_threads; ++i) {
    threads.emplace_back([&] {
      prefault_stack();
      initialized.arrive_and_wait(); // Or just count_down();
      do_work();
    });
  }
  initialized.wait();
  std::cout << "Threads have been initialized, starting to work\n";
  for (auto&& t : threads) {
    t.join();
  }
}

#endif // latch