#include <iostream>
#include <string>
#include <tuple>
#include <gtest/gtest.h>

template <size_t Index, typename Tuple, typename Func> 
void tuple_at(const Tuple& t, Func f) {
  const auto& v = std::get<Index>(t);
  std::invoke(f, v);
}


template <typename Tuple, typename Func, size_t Index = 0> 
void tuple_for_each(const Tuple& t, const Func& f) {
  constexpr auto n = std::tuple_size_v<Tuple>;
  if constexpr(Index < n) {
    tuple_at<Index>(t, f);
    tuple_for_each<Tuple, Func, Index+1>(t, f);
  }
}

template <typename Tuple, typename Func, size_t Index = 0> 
auto tuple_any_of(const Tuple& t, const Func& f) -> bool { 
  constexpr auto n = std::tuple_size_v<Tuple>; 
  if constexpr(Index < n) { 
    bool success = std::invoke(f, std::get<Index>(t)); 
    if (success) {
      return true;
    }
    return tuple_any_of<Tuple, Func, Index+1>(t, f); 
  } else { 
    return false; 
  } 
}



TEST(TupleAlgorithms, ForEach) {
  auto tpl = std::make_tuple(1, true, std::string{"Jedi"});
  tuple_for_each(tpl, [](const auto& ival) {
    std::cout << ival << " ";
  });
}

TEST(TupleAlgorithms, AnyOf) {
  auto tuple = std::make_tuple(42, 43.0f, 44.0);
  auto contains_44 = tuple_any_of(tuple, [](const auto& v) {
    return v == 44;
  });
  (void)(contains_44);
}
