// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace {

struct Student {

  int year_{};
  int score_{};
  std::string name_{};
  // …
};

auto get_max_score_copy(const std::vector<Student>& students, int year) {
  auto by_year = [=](const auto& s) { return s.year_ == year; };
  // The student list needs to be copied in
  // order to filter on the year
  auto v = std::vector<Student>{};
  std::ranges::copy_if(students, std::back_inserter(v), by_year);
  auto it = std::ranges::max_element(v, std::less{}, &Student::score_);
  return it != v.end() ? it->score_ : 0;
}

auto get_max_score_for_loop(const std::vector<Student>& students, int year) {
  auto max_score = 0;
  for (const auto& student : students) {
    if (student.year_ == year) {
      max_score = std::max(max_score, student.score_);
    }
  }
  return max_score;
}

auto max_value(auto&& range) {
  const auto it = std::ranges::max_element(range);
  return it != range.end() ? *it : 0;
}
auto get_max_score(const std::vector<Student>& students, int year) {
  const auto by_year = [=](auto&& s) { return s.year_ == year; };
  return max_value(students | std::views::filter(by_year) |
                   std::views::transform(&Student::score_));
}

auto get_max_score_explicit_views(const std::vector<Student>& s, int year) {
  auto by_year = [=](const auto& s) { return s.year_ == year; };
  auto v1 = std::ranges::ref_view{s}; // Wrap container in a view
  auto v2 = std::ranges::filter_view{v1, by_year};
  auto v3 = std::ranges::transform_view{v2, &Student::score_};
  auto it = std::ranges::max_element(v3);
  return it != v3.end() ? *it : 0;
}

} // namespace

TEST(Student, GetMaxScore) {

  auto students = std::vector<Student>{
      {3, 120, "Niki"},
      {2, 140, "Karo"},
      {3, 190, "Sirius"},
      {2, 110, "Rani"},
  };

  {
    auto score = get_max_score_copy(students, 2); // score is 140
    ASSERT_EQ(score, 140);
  }

  {
    auto score = get_max_score_for_loop(students, 2); // score is 140
    ASSERT_EQ(score, 140);
  }

  {
    auto score = get_max_score(students, 2); // score is 140
    ASSERT_EQ(score, 140);
  }

  {
    auto score = get_max_score_explicit_views(students, 2); // score is 140
    ASSERT_EQ(score, 140);
  }
}

#endif // ranges