#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <version>

#if defined(__cpp_lib_interpolate)

template <typename T>
auto lin_value(T start, T stop, std::size_t index, std::size_t n) {
  assert(n > 1 && index < n);
  const auto amount = static_cast<T>(index) / (n - 1);
  const auto v = std::lerp(start, stop, amount); // C++20
  return v;
}

#else

// Fallback if std::lerp() is missing from cmath
// Prefer the version above using std::lerp()
template <typename T>
auto lin_value(T start, T stop, std::size_t index, std::size_t n) {
  assert(n > 1 && index < n);
  const auto amount = static_cast<T>(index) / (n - 1);
  const auto v = start + amount * (stop - start);
  return v;
}

#endif // __cpp_lib_interpolate