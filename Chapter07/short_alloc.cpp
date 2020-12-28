#include <iostream>
#include <set>
#include <gtest/gtest.h>
#include "arena.h"


template <class T, size_t N>
struct ShortAlloc {
  using value_type = T;
  using arena_type = Arena<N>;
  ShortAlloc(const ShortAlloc&) = default;
  ShortAlloc& operator=(const ShortAlloc&) = default;
  ShortAlloc(arena_type& arena) noexcept : arena_{&arena} { }
  template <class U>
  ShortAlloc(const ShortAlloc<U, N>& other) noexcept
  : arena_{other.arena_} {}
  template <class U> struct rebind {
    using other = ShortAlloc<U, N>;
  };
  auto allocate(size_t n) -> T* {
    return reinterpret_cast<T*>(arena_->allocate(n*sizeof(T)));
  }
  auto deallocate(T* p, size_t n) noexcept -> void {
    arena_->deallocate(reinterpret_cast<std::byte*>(p), n*sizeof(T));
  }
  template <class U, size_t M>
  auto operator==(const ShortAlloc<U, M>& other) const noexcept {
    return N == M && arena_ == other.arena_;
  }
  template <class U, size_t M>
  auto operator!=(const ShortAlloc<U, M>& other) const noexcept {
    return !(*this == other);
  }
  template <class U, size_t M> friend struct ShortAlloc;

private:
  arena_type* arena_;
};

TEST(ShortAlloc, SmallSet) {
  using SmallSet = std::set<int, std::less<int>, ShortAlloc<int, 512>>;

  auto&& stack_arena = SmallSet::allocator_type::arena_type{}; // [auto&& is needed in current version of MSVC]
  auto unique_numbers = SmallSet{stack_arena};

  unique_numbers.insert(3);
  unique_numbers.insert(4);
  unique_numbers.insert(2);
  unique_numbers.insert(4);
  unique_numbers.insert(1);
  unique_numbers.insert(3);
  unique_numbers.insert(3);

  // Print unique numbers
  for (const auto& number : unique_numbers) {
    std::cout << number << '\n';
  }

  ASSERT_EQ(4, unique_numbers.size());
  auto unique_number = unique_numbers.begin();
  for (size_t i = 1; i <= unique_numbers.size(); ++i) {
      ASSERT_EQ(i, *unique_number);
      ++unique_number;
  }
}
