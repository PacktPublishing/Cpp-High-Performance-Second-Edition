#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

TEST(TypeTraits, Categories) {
  [[maybe_unused]] auto same_type = std::is_same_v<uint8_t, unsigned char>;
  const auto is_float_or_double = std::is_floating_point_v<decltype(3.f)>;

  static_assert(is_float_or_double);
}

TEST(TypeTraits, UserDefinedTypes) {
  class Planet {};
  class Mars : public Planet {};
  class Sun {};

  static_assert(std::is_base_of_v<Planet, Mars>);
  static_assert(!std::is_base_of_v<Planet, Sun>);
}


template <typename T> 
auto sign_func(const T& v) -> int {
  if (std::is_unsigned_v<T>) {
    return 1;
  }
  return v < 0 ? -1 : 1;
}

TEST(TypeTraits, SignFunction) {
  {
    auto unsigned_value = uint32_t{42};
    auto sign = sign_func(unsigned_value);
    ASSERT_EQ(1, sign);
  }
  {
    auto signed_value = int32_t{-42};
    auto sign = sign_func(signed_value);
    ASSERT_EQ(-1, sign);
  }
}
