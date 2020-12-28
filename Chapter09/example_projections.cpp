// Requires C++20
#include <version>
#if defined(__cpp_lib_ranges)

#include <ranges>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace {

struct Player {
  std::string name_;
  int level_{};
  int score_{};
  // etc…
};

auto create_players() {
  auto players = std::vector<Player>{};
  players.emplace_back("Kai", 34, 23092);
  players.emplace_back("Ali", 56, 43423);
  players.emplace_back("Mel", 34, 12981);
  return players;
}

} // namespace

TEST(Projections, CompareUsingIfElse) {

  auto players = create_players();

  auto cmp = [](const Player& lhs, const Player& rhs) {
    if (lhs.level_ == rhs.level_) {
      return lhs.score_ < rhs.score_;
    } else {
      return lhs.level_ < rhs.level_;
    }
  };

  std::sort(players.begin(), players.end(), cmp);
  ASSERT_EQ(34, players.front().level_);
  ASSERT_EQ(12981, players.front().score_);
}

TEST(Projections, CompareUsingTie) {

  auto players = create_players();

  auto cmp = [](const Player& lhs, const Player& rhs) {
    auto p1 = std::tie(lhs.level_, lhs.score_); // Projection
    auto p2 = std::tie(rhs.level_, rhs.score_); // Projection
    return p1 < p2;
  };
  std::sort(players.begin(), players.end(), cmp);

  ASSERT_EQ(34, players.front().level_);
  ASSERT_EQ(12981, players.front().score_);
}

TEST(Projections, CompareUsingTieAndRanges) {

  auto players = create_players();
  std::ranges::sort(players, std::less{}, [](const Player& p) {
    return std::tie(p.level_, p.score_);
  });

  ASSERT_EQ(34, players.front().level_);
  ASSERT_EQ(12981, players.front().score_);
}

#endif // ranges