#include "chapter_13.h"
#ifdef SUPPORTS_COROUTINES

#ifdef _MSC_VER
  #pragma warning(disable: 4499) // 'static' : an explicit specialization cannot have a storage class(ignored)
#endif

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <chrono>
#include <iostream>

using namespace std::chrono;
namespace asio = boost::asio;

using asio::ip::tcp;

auto serve_client(tcp::socket socket) -> asio::awaitable<void> {
  std::cout << "New client connected\n";
  auto ex = co_await asio::this_coro::executor;
  auto timer = asio::system_timer{ex};
  auto counter = 0;
  while (true) {
    try {
      auto s = std::to_string(counter) + "\n";
      const auto buf = asio::buffer(s.data(), s.size());
      auto n = co_await async_write(socket, buf, asio::use_awaitable);
      std::cout << "Wrote " << n << " byte(s)\n";
      ++counter;
      timer.expires_from_now(100ms);
      co_await timer.async_wait(asio::use_awaitable);
    } catch (...) {
      // Error or client disconnected
      break;
    }
  }
}

auto listen(tcp::endpoint endpoint) -> asio::awaitable<void> {
  try {
    auto ex = co_await asio::this_coro::executor;
    auto a = tcp::acceptor{ex, endpoint};
    std::cout << "Listening on port " << endpoint.port() << '\n';
    while (true) {
      auto socket = co_await a.async_accept(asio::use_awaitable);
      auto session = [s = std::move(socket)]() mutable {
        auto awaitable = serve_client(std::move(s));
        return awaitable;
      };
      asio::co_spawn(ex, std::move(session), asio::detached);
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << "\n";
    std::terminate();
  }
}

int main() {
  auto server = [] {
    auto endpoint = tcp::endpoint{tcp::v4(), 37259};
    auto awaitable = listen(endpoint);
    return awaitable;
  };
  auto ctx = asio::io_context{};
  asio::co_spawn(ctx, server, asio::detached);
  ctx.run(); // Run event loop from main thread
}

#endif // SUPPORTS_COROUTINES
