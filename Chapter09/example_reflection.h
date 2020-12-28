#pragma once

#include <functional>
#include <string>
#include <utility>

class Player {
public:
  Player(std::string name, int level, int score)
      : name_{std::move(name)}, level_{level}, score_{score} {}

  auto reflect() const { return std::tie(name_, level_, score_); }

private:
  std::string name_;
  int level_{};
  int score_{};
};

template <size_t Index, typename Tuple, typename Func>
void tuple_at(const Tuple& t, Func f) {
  const auto& v = std::get<Index>(t);
  std::invoke(f, v);
}

template <typename Tuple, typename Func, size_t Index = 0>
void tuple_for_each(const Tuple& t, const Func& f) {

  constexpr auto n = std::tuple_size_v<Tuple>;
  if constexpr (Index < n) {
    tuple_at<Index>(t, f);
    tuple_for_each<Tuple, Func, Index + 1>(t, f);
  }
}
