#include <gtest/gtest.h>

#include <iostream>
#include <memory>

struct User {
  User(const std::string& name) : name_(name) { }
  std::string name_;
};

// "Old" placement new
TEST(PlacementNew, MallocAllocate) {
  auto* memory = std::malloc(sizeof(User));
  auto* user = new (memory) User("john");

  ASSERT_EQ("john", user->name_);

  user->~User();
  std::free(memory);
}

// C++17
TEST(PlacementNew, UninitializedFill) {
  auto* memory = std::malloc(sizeof(User));
  auto* user_ptr = reinterpret_cast<User*>(memory);
  std::uninitialized_fill_n(user_ptr, 1, User{"john"});
  ASSERT_EQ("john", user_ptr->name_);
  std::destroy_at(user_ptr);
  std::free(memory);
}

// std::construct_at() is not available in Clang libc++
#if !defined(_LIBCPP_VERSION)

TEST(PlacementNew, ConstructAt) {
  auto* memory = std::malloc(sizeof(User));
  auto* user_ptr = reinterpret_cast<User*>(memory);
  std::construct_at(user_ptr, User{"john"});           // Requires C++20
  ASSERT_EQ("john", user_ptr->name_);                  
  std::destroy_at(user_ptr);
  std::free(memory);
}

#endif // not libc++