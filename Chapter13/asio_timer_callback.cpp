#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono;
namespace asio = boost::asio;

TEST(BoostAsio, TimerExample) {
  auto ctx = asio::io_context{};
  auto timer = asio::system_timer{ctx};
  timer.expires_from_now(1000ms);
  timer.async_wait([](auto /*error*/) { // Callback
    // Ignore errors..
    std::cout << "Hello from delayed callback\n";
  });
  std::cout << "Hello from main\n";
  ctx.run();
}
