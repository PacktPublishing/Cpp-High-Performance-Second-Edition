#include <gtest/gtest.h>

#include <iostream>

namespace {

struct Bear {
  auto roar() const { std::cout << "roar\n"; }
};
struct Duck {
  auto quack() const { std::cout << "quack\n"; }
};

// The function speak can't be used with Bear and Duck class
template <typename Animal> 
auto speak(const Animal& a) {
  if (std::is_same_v<Animal, Bear>) {
    a.roar();
  } else if (std::is_same_v<Animal, Duck>) {
    a.quack();
  }
}

template <typename Animal> 
void constexpr_if_speak(const Animal& a) {
  if constexpr (std::is_same_v<Animal, Bear>) {
    a.roar();
  } else if constexpr (std::is_same_v<Animal, Duck>) {
    a.quack();
  }
}

} // namespace

TEST(ConstexprIfAnimals, Speak) {
  Bear bear;
  Duck duck;

  constexpr_if_speak(bear);
  constexpr_if_speak(duck);

  // Won't compile because constexpr if is not used
  // speak(bear);
  // speak(duck);
}
