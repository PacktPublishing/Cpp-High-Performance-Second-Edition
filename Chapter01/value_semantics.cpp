#include <cassert>
#include <gtest/gtest.h>
#include <set>
#include <string>

//
// This example demonstrates the use of value semantics.
// The set of toppings, t, is passed by value and
// completely copied and encapsulated by the Bagel class.
//

class Bagel {
public:
  Bagel(std::set<std::string> ts) : toppings_(std::move(ts)) {}

private:
  std::set<std::string> toppings_;
};

TEST(ValueSemantics, CreateBagels) {

  auto t = std::set<std::string>{};
  t.insert("salt");
  auto a = Bagel{t};

  // ‘a’ is not affected
  // when adding pepper
  t.insert("pepper");

  // ‘a’ will have salt
  // ‘b’ will have salt & pepper
  auto b = Bagel{t};

  // No bagel is affected
  t.insert("oregano");
}
