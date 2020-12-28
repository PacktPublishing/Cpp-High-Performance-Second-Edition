#include <cassert>
#include <gtest/gtest.h>
#include <vector>

//
// This example demonstrates how C++ can minimize
// heap allocations by:
// 1) Creating objects on the stack
// 2) Allocate multiple objects in one heap allocation
//

struct Car {
  int doors_{};
};

auto some_func() {
  auto num_doors = 2;
  auto car1 = Car{num_doors};
  auto car2 = Car{num_doors};
}

auto car_list() {
  const auto n = 7;
  auto cars = std::vector<Car>{};
  cars.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    cars.push_back(Car{});
  }
}

TEST(HeapAllocations, Cars) {
  some_func();
  car_list();
}
