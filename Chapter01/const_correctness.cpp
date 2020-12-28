#include <cassert>
#include <vector>
#include <gtest/gtest.h>

//
// This example demonstrates how using const can
// help us distinguish between mutating and
// nonmutating functions.
//

class Person {
public:
  auto age() const {
    return age_;
  }
  auto set_age(int age) {
    age_ = age;
  }

private:
  int age_{};
};


class Team {
public:
  auto& leader() const {
    return leader_;
  }
  auto& leader() {
    return leader_;
  }

private:
  Person leader_{};
};


// This function is guaranteed to not mutate the teams
// since teams is declared const.
// Trying to mutate the teams will generate a compilation error.
auto nonmutating_func(const std::vector<Team>& teams) {
  auto tot_age = int{0};

  // Compiles, both leader() and age() are declared const
  for (const auto& team: teams)
    tot_age += team.leader().age();

  // Will not compile, set_age requires a mutable object
  //  for(auto& team: teams)
  //    team.leader().set_age(20);
}


// This function may mutate the Team objects.
auto mutating_func(std::vector<Team>& teams) {
  auto tot_age = int{0};

  // Compiles, const functions can be called on mutable objects
  for (const auto& team: teams)
    tot_age += team.leader().age();

  // Compiles, planes is mutable
  for (auto& team: teams)
    team.leader().set_age(20);
}

TEST(ConstCorrectness, Nonmutating) {
  const auto teams = std::vector<Team>{};
  nonmutating_func(teams);
  // mutating_func(teams); // Will not comiple, teams vector is const
}

TEST(ConstCorrectness, Mutating) {
  auto teams = std::vector<Team>{};
  nonmutating_func(teams);
  mutating_func(teams);
}

