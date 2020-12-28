// Requires C++20
#include <version>
#if defined(__cpp_concepts) && defined(__cpp_lib_concepts)

#include <gtest/gtest.h>

#include <cmath>
#include <concepts>

//
// Defining concepts
//

namespace {

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <typename T>
concept Number = FloatingPoint<T> || std::is_integral_v<T>;

template <typename T>
concept Range = requires(T& t) {
  begin(t);
  end(t);
};

} // namespace

//
// Constraining types with concepts
//

namespace {

template <typename T>
requires std::integral<T> auto mod(T v, T n) {
  return v % n;
}

// Using concepts with abbreviated function templates
// (Note that Visual Studio does not "auto" as function paramter at the time of writing this
#ifndef _MSC_VER
std::integral auto mod_abbreviated(std::integral auto v, std::integral auto n) {
  return v % n;
}
#else
template <std::integral I>
std::integral auto mod_abbreviated(I v, I n) {
  return v % n;
}
#endif

template <typename T>
requires std::integral<T> struct Foo {
  T value;
};

} // namespace

TEST(ConstraintsAndConcepts, ConstrainingFunctionTemplates) {
  auto x = mod(5, 2); // OK: Integral types
  ASSERT_EQ(1, x);

  // Compilation error, not integral types
  // auto y = mod(5.f, 2.f);
}

TEST(ConstraintsAndConcepts, ConstrainingAbbreviatedFunctionTemplates) {
  auto x = mod_abbreviated(5, 2); // OK: Integral types
  ASSERT_EQ(1, x);

  // Compilation error, not integral types
  // auto y = mod_abbreviated(5.f, 2.f);
}

TEST(ConstraintsAndConcepts, ConstrainingClassTemplates) {
  auto x = Foo{5};
  ASSERT_EQ(5, x.value);

  // Compilation error, not integral types
  // auto y = Foo{5.f};
}

//
// Function overloading
//

namespace {

template <typename T>
auto generic_mod_if_constexpr(T v, T n) -> T {
  if constexpr (std::is_floating_point_v<T>) {
    return std::fmod(v, n);
  } else {
    return v % n;
  }
}


// Instead of using if constexpr, we can use
// function overloading

template <std::integral T>
T generic_mod(T v, T n) { // Integral version
  return v % n;
}

template <std::floating_point T>
T generic_mod(T v, T n) { // Floating point version
  return std::fmod(v, n);
}

} // namespace

TEST(ConstraintsAndConcepts, FunctionOverloading) {
  // Using an integral type
  {
    auto const value = 7;
    auto const modulus = 5;
    auto const x = generic_mod(value, modulus);
    auto const y = generic_mod_if_constexpr(value, modulus);
    ASSERT_EQ(2, x);
    ASSERT_EQ(2, y);
  }
  // Using a floating point type
  {
    auto const value = 1.5f;
    auto const modulus = 1.0f;
    auto const x = generic_mod(value, modulus);
    auto const y = generic_mod_if_constexpr(value, modulus);
    ASSERT_FLOAT_EQ(0.5f, x);
    ASSERT_FLOAT_EQ(0.5f, y);
  }
}

#endif // concepts