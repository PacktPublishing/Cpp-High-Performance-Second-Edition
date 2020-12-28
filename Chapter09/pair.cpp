#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <gtest/gtest.h>

TEST(Pair, MinMax) {
  std::pair<int, int> v = std::minmax({4, 3, 2, 4, 5, 1});
  std::cout << v.first << " " << v.second; // Outputs: "1 5"
}

TEST(Pair, Map) {
  auto scores = std::map<std::string, int>{};
  scores.insert(std::pair{"Neo", 12}); // Correct but ineffecient
  scores.emplace("Tri", 45);           // Use emplace() instead
  scores.emplace("Ari", 33);

  for (auto&& it : scores) { // "it" is a std::pair
    auto key = it.first;
    auto val = it.second;
    std::cout << key << ": " << val << '\n';
  }
}

TEST(Pair, MapInitializerListAndStructuredBindings) {

  auto scores = std::map<std::string, int>{{"Neo", 12}, // Initializer lists
                                           {"Tri", 45},
                                           {"Ari", 33}};

  for (auto&& [key, val] : scores) { // Structured bindings
    std::cout << key << ": " << val << '\n';
  }
}