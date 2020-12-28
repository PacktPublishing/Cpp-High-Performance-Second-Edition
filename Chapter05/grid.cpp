// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>
#include <vector>

namespace {

struct Grid {
  Grid(size_t w, size_t h) : w_{w}, h_{h} { data_.resize(w * h); }
  auto get_row_v1(size_t y); // Returns iterator pairs
  auto get_row_v2(size_t y); // Returns range using subrange
  auto get_row_v3(size_t y); // Returns range using counted
  std::vector<int> data_{};
  size_t w_{};
  size_t h_{};
};

auto Grid::get_row_v1(size_t y) {
  auto left = data_.begin() + w_ * y;
  auto right = left + w_;
  return std::make_pair(left, right);
}

auto Grid::get_row_v2(size_t y) {
  auto first = data_.begin() + w_ * y;
  auto sentinel = first + w_;
  return std::ranges::subrange{first, sentinel};
}

auto Grid::get_row_v3(size_t y) {
  auto first = data_.begin() + w_ * y;
  return std::views::counted(first, w_);
}

} // namespace

TEST(Grid, CountFivesUsingIteratorPairs) {
  auto grid = Grid{10, 10};
  auto y = 3;

  auto row = grid.get_row_v1(y);

  std::generate(row.first, row.second, std::rand);
  auto num_fives = std::count(row.first, row.second, 5);
}

TEST(Grid, CountFivesUsingSubrange) {
  auto grid = Grid{10, 10};
  auto y = 3;

  auto row = grid.get_row_v2(y);

  std::ranges::generate(row, std::rand);
  auto num_fives = std::ranges::count(row, 5);
}

TEST(Grid, CountFivesUsingCounted) {
  auto grid = Grid{10, 10};
  auto y = 3;

  auto row = grid.get_row_v3(y);

  std::ranges::generate(row, std::rand);
  auto num_fives = std::ranges::count(row, 5);
}

#endif // ranges