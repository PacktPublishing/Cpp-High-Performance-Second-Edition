#include <cassert>
#include <cmath>
#include <gtest/gtest.h>

//
// This example demonstrates the use of references
// versus pointers in function signatures.
//

struct Sphere {
  auto radius() const {
    return 0.05f;
  }
};

auto get_volume1(const Sphere& s) {
  auto cube = std::pow(s.radius(), 3);
  auto pi = 3.14f;
  return (pi * 4.f / 3.f) * cube;
}

auto get_volume2(const Sphere* s) {
  auto rad = s ? s->radius() : 0.f;
  auto cube = std::pow(rad, 3);
  auto pi = 3.14f;
  return (pi * 4.f / 3.f) * cube;
}

TEST(References, PassingObject) {
  // Using references
  auto sphere = Sphere{};
  auto volume = get_volume1(sphere);
  ASSERT_GT(volume, 0.f);
}

TEST(References, PassingNullptr) {
  // Using pointers
  auto sphere = Sphere{};
  auto sphere_ptr = std::addressof(sphere);
  auto volume = get_volume2(sphere_ptr);
  ASSERT_GT(volume, 0.f);

  volume = get_volume2(nullptr);
  ASSERT_EQ(volume, 0.f);
}
