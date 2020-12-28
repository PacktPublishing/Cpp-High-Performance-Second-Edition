#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

TEST(HeterogenousContainerOfVariants, Modification) {
  using namespace std::string_literals;

  using VariantType = std::variant<int, std::string, bool>;

  auto container = std::vector<VariantType>{};
  container.push_back(false);
  container.push_back("I am a string"s);
  container.push_back("I am also a string"s);
  container.push_back(13);

  container.pop_back();
  std::reverse(container.begin(), container.end());
}

TEST(HeterogenousContainerOfVariants, AccessingValue) {

  using namespace std::string_literals;

  using VariantType = std::variant<int, std::string, bool>;
  auto v = std::vector<VariantType>{42, "needle"s, true};

  for (const auto& item : v) {
    std::visit([](const auto& x) { std::cout << x << '\n'; }, item);
  }

  auto num_bools = std::count_if(v.begin(), v.end(), [](const auto& item) {
    return std::holds_alternative<bool>(item);
  });

  ASSERT_EQ(1, num_bools);

  auto contains_needle_string =
      std::any_of(v.begin(), v.end(), [](const auto& item) {
        return std::holds_alternative<std::string>(item) &&
               std::get<std::string>(item) == "needle";
      });

  ASSERT_TRUE(contains_needle_string);
}
