#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <tuple>

using namespace std::string_literals; // "..."s

namespace {

auto make_saturn() { return std::tuple{"Saturn"s, 82, true}; }

auto make_earth() {
  struct Planet { std::string name; int n_moons; bool rings; };
  return Planet{"Earth", 1, false};
}

} // namespace

TEST(TupleAccessingElements, StdGet) {
  // Using std::get<N>
  auto t = make_saturn();
  auto name = std::get<0>(t);
  auto n_moons = std::get<1>(t);
  auto rings = std::get<2>(t);
  std::cout << name << ' ' << n_moons << ' ' << rings << '\n';
  // Output: Saturn 82 true

  ASSERT_EQ("Saturn", name);
  ASSERT_EQ(82, n_moons);
  ASSERT_EQ(true, rings);
}

TEST(TupleAccessingElements, StdTie) {

  // Using std::tie
  auto name = std::string{};
  auto n_moons = int{};
  auto rings = bool{};
  std::tie(name, n_moons, rings) = make_saturn();
  std::cout << name << ' ' << n_moons << ' ' << rings << '\n';

  ASSERT_EQ("Saturn", name);
  ASSERT_EQ(82, n_moons);
  ASSERT_EQ(true, rings);
}

TEST(TupleAccessingElements, StructuredBinding) {

  const auto& [name, n_moons, rings] = make_saturn();

  std::cout << name << ' ' << n_moons << ' ' << rings << '\n';

  ASSERT_EQ("Saturn", name);
  ASSERT_EQ(82, n_moons);
  ASSERT_EQ(true, rings);
}

TEST(TupleAccessingElements, ForLoopWithStructuredBinding) {

  auto planets = {std::tuple{"Mars"s, 2, false},
                  std::tuple{"Neptune"s, 14, true}};
  for (auto&& [name, n_moons, rings] : planets) {
    std::cout << name << ' ' << n_moons << ' ' << rings << '\n';
  }
}

TEST(TupleAccessingElements, StructWithNamedVariables) {

  auto p = make_earth();
  std::cout << p.name << ' ' << p.n_moons << ' ' << p.rings << '\n';
  
  ASSERT_EQ("Earth", p.name);
  ASSERT_EQ(1, p.n_moons);
  ASSERT_EQ(false, p.rings);
}