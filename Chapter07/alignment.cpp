#include <gtest/gtest.h>

#include <bit>
#include <iostream>

bool is_aligned(void* ptr, std::size_t alignment) {
  assert(ptr != nullptr);
  // assert(std::has_single_bit(alignment)); // Power of 2   // Requires C++20
  
  auto s = std::numeric_limits<std::size_t>::max();
  // void* aligned_ptr = ptr;
  auto* aligned_ptr = ptr;
  std::align(alignment, 1, aligned_ptr, s);
  return ptr == aligned_ptr;
}

// Memory returned from new is correctly aligned for std::max_align_t
// which means that it is also correctly aligned for any scalar type
TEST(Alignment, PointerAddress) {
  auto* p = new char{};
  auto max_alignment = alignof(std::max_align_t);
  ASSERT_TRUE(is_aligned(p, max_alignment));
}

// Memory returned from new is correctly aligned for std::max_align_t
// which means that it is also correctly aligned for any scalar type
TEST(Alignment, SingleByteAllocation) {
  auto* p1 = new char{};
  auto* p2 = new char{};
  auto a1 = reinterpret_cast<std::uintptr_t>(p1);
  auto a2 = reinterpret_cast<std::uintptr_t>(p2);
  std::cout << "Num bytes between p1 and p2: " << a2 - a1 << '\n';
  // auto max_alignment = alignof(std::max_align_t);
  // ASSERT_TRUE(is_aligned(p, max_alignment));
}

// A type with custom alignment
struct alignas(64) CacheLine {
  std::byte data[64];
};

// Custom aligned types
TEST(Alignment, CacheLine) {
  auto x = CacheLine{};
  auto y = CacheLine{};
  ASSERT_TRUE(is_aligned(&x, 64));
  ASSERT_TRUE(is_aligned(&y, 64));

  auto* p = new CacheLine{};
  ASSERT_TRUE(is_aligned(p, 64));
}

// new with alignment
TEST(Alignment, AlignedNew) {

  // The overloads:
  // void* operator new(size_t isize, std::align_val_t al)
  // void operator delete(void* p, std::align_val_t al) noexcept
  // will be invoked when using new and delete here.

  constexpr auto ps = std::size_t{4096};          // Page size

  struct alignas(ps) Page {
    std::byte data_[ps];
  };  

  auto* page = new Page{};                        // Memory page
  ASSERT_TRUE(is_aligned(page, ps));              // True

  delete page;
}