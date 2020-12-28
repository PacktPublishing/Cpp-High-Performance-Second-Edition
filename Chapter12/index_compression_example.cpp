// Requires C++20
#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include "generator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

template <typename Range>
 auto gap_encode(Range& ids) -> Generator<int> {
  auto last_id = 0;
  for (auto id : ids) {
    const auto gap = id - last_id;
    last_id = id;
    co_yield gap;
  }
}

template <typename Range>
auto gap_decode(Range& gaps) -> Generator<int> {
  int last_id = 0;
  for (auto gap : gaps) {
    const auto id = gap + last_id;
    co_yield id;
    last_id = id;
  }
}

auto vb_encode_num(int n) -> Generator<std::uint8_t> {
  for (auto cont = std::uint8_t{0}; cont == 0;) {
    auto b = static_cast<std::uint8_t>(n % 128);
    n = n / 128;
    cont = (n == 0) ? 128 : 0;
    co_yield(b + cont);
  }
}

template <typename Range>
auto vb_encode(Range& r) -> Generator<std::uint8_t> {
  for (auto n : r) {
    auto bytes = vb_encode_num(n);
    for (auto b : bytes) {
      co_yield b;
    }
  }
}

template <typename Range>
auto vb_decode(Range& bytes) -> Generator<int> {
  int n = 0;
  int weight = 1;
  for (auto b : bytes) {
    if (b < 128) { // Cont
      n += b * weight;
      weight *= 128;
    } else {
      // Process last byte and yield
      n += (b - 128) * weight;
      co_yield n;
      n = 0;      // Reset
      weight = 1; // Reset
    }
  }
}

template <typename Range>
auto compress(Range& ids) -> Generator<int> {
  auto gaps = gap_encode(ids);
  auto bytes = vb_encode(gaps);
  for (auto b : bytes) {
    co_yield b;
  }
}

template <typename Range>
auto decompress(Range& bytes) -> Generator<int> {
  auto gaps = vb_decode(bytes);
  auto ids = gap_decode(gaps);
  for (auto id : ids) {
    co_yield id;
  }
}

template <typename Range>
void write(const std::string& path, Range& bytes) {
  auto out = std::ofstream{path, std::ios::out | std::ofstream::binary};
  std::ranges::copy(bytes.begin(), bytes.end(),
                    std::ostreambuf_iterator<char>(out));
}

auto read(std::string path) -> Generator<std::uint8_t> {
  auto in = std::ifstream{path, std::ios::in | std::ofstream::binary};
  auto it = std::istreambuf_iterator<char>{in};
  const auto end = std::istreambuf_iterator<char>{};
  for (; it != end; ++it) {
    co_yield *it;
  }
}

TEST(IndexCompressionExample, GapEncode) {

  auto ids = std::vector{10, 11, 12, 14};
  auto gaps = gap_encode(ids);
  for (auto&& gap : gaps) {
    std::cout << gap << ", ";
  }
} // Prints: 10, 1, 1, 2,

TEST(IndexCompressionExample, CompressAndDecompress) {
  {
    auto documents = std::vector{367, 438, 439, 440};
    auto bytes = compress(documents);
    write("values.bin", bytes);
  }
  {
    auto bytes = read("values.bin");
    auto documents = decompress(bytes);
    auto os = std::ostringstream{};
    for (auto doc : documents) {
      os << doc << ", ";
    }
    ASSERT_EQ("367, 438, 439, 440, ", os.str());
  }
}

#endif // ranges

#endif // SUPPORTS_COROUTINES
