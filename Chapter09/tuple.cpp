#include <string>
#include <tuple>
#include <gtest/gtest.h>

// Example of how compiler can generate tuple
class Tuple {
public:
  int data0_{};
  std::string data1_{};
  bool data2_{};
};

template <size_t Index, typename Tuple>
auto& get(const Tuple& tpl) {
  if constexpr(Index == 0) {
    return tpl.data0_;
  }
  else if constexpr(Index == 1) {
    return tpl.data1_;
  }
  else if constexpr(Index == 2) {
    return tpl.data2_;
  }
}

TEST(Tuple, StdTuple) {
  const auto tuple = std::tuple{42, std::string{"hi"}, true};
  const auto number = std::get<0>(tuple);
  const auto str = std::get<1>(tuple);
  const auto boolean = std::get<2>(tuple);

  ASSERT_EQ(42, number);
  ASSERT_EQ("hi", str);
  ASSERT_EQ(true, boolean);
}

TEST(Tuple, StdMakeTuple) {
  const auto tuple = std::make_tuple(42, std::string{"hi"}, true);
  const auto number = std::get<0>(tuple);
  const auto str = std::get<1>(tuple);
  const auto boolean = std::get<2>(tuple);

  ASSERT_EQ(42, number);
  ASSERT_EQ("hi", str);
  ASSERT_EQ(true, boolean);
}

TEST(Tuple, CustomTuple) {
  const auto tuple = Tuple{42, std::string{"hi"}, true};
  const auto number = get<0>(tuple);
  const auto str = get<1>(tuple);
  const auto boolean = get<2>(tuple);

  ASSERT_EQ(42, number);
  ASSERT_EQ("hi", str);
  ASSERT_EQ(true, boolean);
}
