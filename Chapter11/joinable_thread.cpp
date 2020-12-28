#include <version>
#if defined(__cpp_lib_jthread)

#include <gtest/gtest.h>
#include <iostream>
#include <thread>

namespace {
void print() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Thread ID: " << std::this_thread::get_id() << '\n';
}
TEST(JoinableThread, ToConsole) {
  std::cout << "main begin\n";
  auto joinable_thread = std::jthread{print};
  std::cout << "main end\n";
} // OK: jthread will join automatically

} // namespace

#endif // jthread