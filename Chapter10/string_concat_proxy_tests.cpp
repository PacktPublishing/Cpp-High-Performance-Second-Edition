#include "string_concat_proxy.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>



TEST(ConcatProxy, StdConcatenateAndCompareStrings) {
  auto a = std::string{"Cole"};
  auto b = std::string{"Porter"};
  auto c = std::string{"ColePorter"};
  auto is_cole_porter = is_concat_equal(a, b, c);
  ASSERT_TRUE(is_cole_porter);
}

TEST(ConcatProxy, ConcatenateAndCompareStrings) {
  auto a = String{"Cole"};
  auto b = String{"Porter"};
  auto c = String{"ColePorter"};
  auto is_cole_porter = (a + b) == c;
  ASSERT_TRUE(is_cole_porter);
}

TEST(ConcatProxy, ConcatenateStrings) {
  String c = String{"Marc"} + String{"Chagall"};
  ASSERT_TRUE(c.str_ == "MarcChagall");
}




TEST(ConcatProxy, Test) {
  auto a = String{"Marc"};
  auto b = String{"Chagall"};
  auto c = String{"MarcChagall"};
  auto is_equal = (String{"Marc"} +  String{"Chagall"}) == c;
  ASSERT_TRUE(is_equal);
  is_equal = (a + b) == String{"MarcChagall"};
  ASSERT_TRUE(is_equal);
}
