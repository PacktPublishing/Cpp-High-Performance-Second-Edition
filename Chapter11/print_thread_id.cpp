#include <gtest/gtest.h>

#include <iostream>
#include <thread>

auto print() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Thread ID: " << std::this_thread::get_id() << '\n';
}

TEST(PrintThreadId, BasicExample) {
  std::thread t1(print);
  std::cout << "Thread ID: " << std::this_thread::get_id() << '\n';
  t1.join();
}
