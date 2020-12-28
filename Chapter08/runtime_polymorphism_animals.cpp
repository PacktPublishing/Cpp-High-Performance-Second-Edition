#include <gtest/gtest.h>

#include <iostream>

namespace {

struct AnimalBase {
  virtual ~AnimalBase() {}
  virtual auto speak() const -> void {}
};

struct Bear : public AnimalBase {
  auto roar() const { std::cout << "roar\n"; }
  auto speak() const -> void override { roar(); }
};

struct Duck : public AnimalBase {
  auto quack() const { std::cout << "quack\n"; }
  auto speak() const -> void override { quack(); }
};

auto speak(const AnimalBase& a) { 
  a.speak(); 
}

} // namespace

TEST(RuntimePolymorphism, Animals) {
  Bear bear;
  Duck duck;

  speak(bear);
  speak(duck);
}
