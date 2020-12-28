#include "example_reflection.h"

#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace {

template <typename T>
concept Reflectable = requires(T& t) {
  t.reflect();
};

template <Reflectable R>
auto& operator<<(std::ostream& os, const R& v) {
  tuple_for_each(v.reflect(), [&os](const auto& m) { os << m << " "; });
  return os;
}

} // namespace

TEST(Reflection, Concepts) {
  auto kai = Player{"Kai", 4, 2568};
  auto ari = Player{"Ari", 2, 1068};

  std::cout << kai; // Prints "Kai 4 2568 "
  std::cout << std::endl;
  std::cout << ari; // Prints "Ari 2 1068 "
  std::cout << std::endl;

  auto os = std::ostringstream{};
  os << kai;
  ASSERT_EQ("Kai 4 2568 ", os.str());
}
