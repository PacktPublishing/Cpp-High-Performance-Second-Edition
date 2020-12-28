 // Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <vector>

TEST(AlgorithmFeatures, RemoveErase) {
  auto v = std::vector{1, 2, 2, 3};
  auto new_end = std::remove(v.begin(), v.end(), 2);
  v.erase(new_end, v.end());

  auto result = std::vector{1, 3};
  ASSERT_EQ(v, result);
}

TEST(AlgorithmFeatures, RemoveEraseUsingRanges) {
  auto v = std::vector{1, 2, 2, 3};
  auto r = std::ranges::remove(v, 2);
  v.erase(r.begin(), r.end());

  auto result = std::vector{1, 3};
  ASSERT_EQ(v, result);
}

TEST(AlgorithmFeatures, UniqueErase) {
  auto v = std::vector{1, 2, 2, 3};
  auto new_end = std::unique(v.begin(), v.end());
  v.erase(new_end, v.end());

  auto result = std::vector{1, 2, 3};
  ASSERT_EQ(v, result);
}

TEST(AlgorithmFeatures, UniqueEraseUsingRanges) {
  auto v = std::vector{1, 2, 2, 3};
  auto r = std::ranges::unique(v);
  v.erase(r.begin(), r.end());

  auto result = std::vector{1, 2, 3};
  ASSERT_EQ(v, result);
}

TEST(AlgorithmFeatures, PreAllocateSpace) {
  auto v = std::vector{1, 2, 3, 4};
  auto squared = std::vector<int>{};
  squared.resize(v.size());
  std::transform(v.begin(), v.end(), squared.begin(),
                 [](int x) { return x * x; });

  auto result = std::vector{1, 4, 9, 16};
  ASSERT_EQ(squared, result);
}

TEST(AlgorithmFeatures, BackInserter) {

  auto square = [](int x) { return x * x; };
  auto v = std::vector{1, 2, 3, 4};

  // Insert into std::vector
  auto squared_vec = std::vector<int>{};
  std::ranges::transform(v, std::back_inserter(squared_vec), square);

  // Insert into std::set
  auto squared_set = std::set<int>{};
  std::ranges::transform(v, std::inserter(squared_set, squared_set.end()),
                         square);
}

// operator<() and operator==()

TEST(AlgorithmFeatures, ComparisonOperators) {

  struct Flower {
    auto operator<=>(const Flower& f) const = default;
    bool operator==(const Flower&) const = default;

    int height_{};
  };

  auto garden = std::vector<Flower>{{67}, {28}, {14}};
  // std::max_element() uses operator<()
  auto tallest = std::max_element(garden.begin(), garden.end());
  // std::find() uses operator==()
  auto perfect = *std::find(garden.begin(), garden.end(), Flower{28});
}

TEST(AlgorithmFeatures, CustomComparatorFunctions) {

  auto names = std::vector<std::string>{"Ralph",  "Lisa", "Homer",
                                        "Maggie", "Apu",  "Bart"};
  std::sort(names.begin(), names.end(),
            [](const std::string& a, const std::string& b) {
              return a.size() < b.size();
            });
  // names is now "Apu", "Lisa", "Bart", "Ralph", "Homer", "Maggie"
  // Find names with length 3
  auto x = std::find_if(names.begin(), names.end(),
                        [](const auto& v) { return v.size() == 3; });
  // x points to "Apu"

  ASSERT_EQ(*x, "Apu");
}

TEST(AlgorithmFeatures, Projections) {

  auto names = std::vector<std::string>{"Ralph",  "Lisa", "Homer",
                                        "Maggie", "Apu",  "Bart"};
  std::ranges::sort(names, std::less<>{}, &std::string::size);
  // names is now "Apu", "Lisa", "Bart", "Ralph", "Homer", "Maggie"
  // Find names with length 3
  auto x = std::ranges::find(names, 3, &std::string::size);

  ASSERT_EQ(*x, "Apu");
}

TEST(AlgorithmFeatures, LambdaProjections) {

  struct Player {
    std::string name{};
    int level{};
    float health{1.0f};
    // …
  };
  auto players = std::vector<Player>{
    {"Aki", 1, 9.f}, 
    {"Nao", 2, 7.f}, 
    {"Rei", 2, 3.f}};
  auto level_and_health = [](const Player& p) {
    return std::tie(p.level, p.health);
  };
  // Order players by level, then health
  std::ranges::sort(players, std::greater<>{}, level_and_health);

  ASSERT_EQ(players.front().name, "Nao");
}

#endif // ranges