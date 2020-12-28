#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include <gtest/gtest.h>

template <size_t Index, typename TupleType, typename Functor>
auto tuple_at(const TupleType& tpl, const Functor& func) -> void {
  const auto& val = std::get<Index>(tpl);
  func(val);
}

template <typename TupleType, typename Functor, size_t Index = 0>
auto tuple_for_each(const TupleType& tpl, const Functor& ifunctor) -> void {
  constexpr auto tuple_size = std::tuple_size_v<TupleType>;
  if constexpr (Index < tuple_size) {
    tuple_at<Index>(tpl, ifunctor);
    tuple_for_each<TupleType, Functor, Index + 1>(tpl, ifunctor);
  }
}

// Makes a string of by one argument
template <typename T> auto make_string(const T& v0) -> std::string {
  auto sstr = std::ostringstream{};
  sstr << v0;
  return sstr.str();
}

// Makes a string of by two arguments
template <typename T0, typename T1>
auto make_string(const T0& v0, const T1& v1) -> std::string {
  return make_string(v0) + " " + make_string(v1);
}

// Makes a string of by three arguments
template <typename T0, typename T1, typename T2>
auto make_string(const T0& v0, const T1& v1, const T2& v2) -> std::string {
  return make_string(v0, v1) + " " + make_string(v2);
}

// Makes a string of by four arguments
template <typename T0, typename T1, typename T2, typename T3>
auto make_string(const T0& v0, const T1& v1, const T2& v2, const T3& v3)
    -> std::string {
  return make_string(v0, v1, v2) + " " + make_string(v3);
}

template <typename... Ts> auto expand_pack(const Ts&... values) {
  auto tuple = std::tie(values...);
}

template <typename... Ts> auto make_string(const Ts&... values) {
  auto sstr = std::ostringstream{};
  // Create a tuple of the variadic parameter pack
  auto tuple = std::tie(values...);
  // Iterate the tuple
  tuple_for_each(tuple, [&sstr](const auto& v) { sstr << v; });
  return sstr.str();
}

TEST(VariadicTemplateParameterPack, MakeString) {
  // Without variadic pack
  auto str1 = make_string(42);
  ASSERT_EQ("42", str1);

  auto str2 = make_string(42, "hi");
  ASSERT_EQ("42 hi", str2);

  auto str3 = make_string(42, "hi", true);
  ASSERT_EQ("42 hi 1", str3);

  auto str4 = make_string(42, "hi", true, false);
  ASSERT_EQ("42 hi 1 0", str4);

  // With variadic pack
  auto str5 = make_string(42, "hi", true, false, 42.0f);
  ASSERT_EQ("42hi1042", str5);
}
