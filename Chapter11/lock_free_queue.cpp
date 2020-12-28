#include <gtest/gtest.h>

#include <array>
#include <atomic>
#include <cassert>
#include <iostream>
#include <optional>
#include <thread>

template <class T, size_t N> class LockFreeQueue {
  std::array<T, N> buffer_{};   // Used by both threads
  std::atomic<size_t> size_{0}; // Used by both threads
  size_t read_pos_{0};          // Used by reader thread
  size_t write_pos_{0};         // Used by writer thread
  static_assert(std::atomic<size_t>::is_always_lock_free);

  bool do_push(T&& t) {
    if (size_.load() == N) {
      return false;
    }
    buffer_[write_pos_] = std::forward<decltype(t)>(t);
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
    return true;
  }

public:
  // Writer thread
  bool push(T&& t) { return do_push(std::move(t)); }
  bool push(const T& t) { return do_push(t); }

  // Reader thread
  auto pop() -> std::optional<T> {
    auto val = std::optional<T>{};
    if (size_.load() > 0) {
      val = std::move(buffer_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
      size_.fetch_sub(1);
    }
    return val;
  }
  auto size() const noexcept { return size_.load(); }
};

constexpr auto max_size = 10000;
constexpr auto done = -1;

TEST(LockFreeQueue, PutAndPop) {
  LockFreeQueue<std::optional<int>, max_size> queue;
  std::vector<int> result;

  auto writer = std::thread{[&queue]() {
    for (auto i = 0; i < 10; ++i) {
      queue.push({i});
    }
    queue.push(std::nullopt);
  }};

  auto reader = std::thread{[&queue, &result]() {
    for (;;) {
      while (queue.size() == 0) {
        // busy wait
      }
      auto element = *queue.pop();
      if (element.has_value()) {
        std::cout << "Got: " << *element << '\n';
        result.push_back(*element);
      } else {
        break;
      }
    }
  }};

  writer.join();
  reader.join();

  ASSERT_EQ(std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), result);
}
