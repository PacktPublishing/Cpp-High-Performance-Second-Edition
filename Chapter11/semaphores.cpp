// Can be tested using clang trunk
#include <version>
#if defined(__cpp_lib_semaphore)

#include <gtest/gtest.h>

#include <array>
#include <iostream>
#include <optional>
#include <semaphore>
#include <string>
#include <thread>

namespace {

template <class T, int N> class BoundedBuffer {
  std::array<T, N> buf_;
  std::size_t read_pos_{};
  std::size_t write_pos_{};
  std::mutex m_;
  std::counting_semaphore<N> n_empty_slots_{N}; // New
  std::counting_semaphore<N> n_full_slots_{0};  // New

  void do_push(auto&& item) {
    // Take one of the empty slots (might block)
    n_empty_slots_.acquire(); // New
    try {
      auto lock = std::unique_lock{m_};
      buf_[write_pos_] = std::forward<decltype(item)>(item);
      write_pos_ = (write_pos_ + 1) % N;
    } catch (...) {
      n_empty_slots_.release();
      throw;
    }
    // Increment and signal that there is one more full slot
    n_full_slots_.release(); // New
  }

public:
  void push(const T& item) { do_push(item); }
  void push(T&& item) { do_push(std::move(item)); }

  auto pop() {
    // Take one of the full slots (might block)
    n_full_slots_.acquire(); // New
    auto item = std::optional<T>{};
    try {
      auto lock = std::unique_lock{m_};
      item = std::move(buf_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
    } catch (...) {
      n_full_slots_.release();
      throw;
    }
    // Increment and signal that there is one more empty slot
    n_empty_slots_.release(); // New
    return std::move(*item);
  }
};

} // namespace

TEST(Semaphores, BoundedBuffer) {

  auto buffer = BoundedBuffer<std::string, 5>{};
  auto sentinel = std::string{""};

  auto producer = std::thread{[&]() {
    buffer.push("A");
    buffer.push("B");
    buffer.push("C");
    buffer.push(sentinel);
  }};

  auto consumer = std::thread{[&]() {
    for (;;) {
      auto s = buffer.pop();
      if (s != sentinel) {
        std::cout << "Got: " << s << '\n';
      } else {
        break;
      }
    }
  }};

  producer.join();
  consumer.join();
}

#endif // semaphore