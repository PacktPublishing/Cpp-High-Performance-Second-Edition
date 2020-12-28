#include <string>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <gtest/gtest.h>

//
// This example demonstrates how to use an unordered_set.
// To combine hashes we use boost::hash_combine.
//

class Person {
public:
  Person(const std::string& name, int age)
  : name_{name}, age_{age} { }

  auto name() const {
    return name_;
  }
  auto age() const {
    return age_;
  }

private:
  std::string name_{};
  int age_{};
};

auto person_eq = [](const Person& lhs, const Person& rhs) {
  return lhs.name() == rhs.name() && lhs.age() == rhs.age();
};

auto person_hash = [](const Person& person) {
  auto seed = size_t{0};
  // See also http://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
  boost::hash_combine(seed, person.name());
  boost::hash_combine(seed, person.age());
  return seed;
};

TEST(UnorderedSets, CheckElementPresence) {
  using Set = std::unordered_set<Person, decltype(person_hash), decltype(person_eq)>;

  auto persons = Set{100, person_hash, person_eq};
  persons.emplace("John", 32);
  persons.emplace("Anne", 45);

  ASSERT_EQ(0, persons.count(Person{"John", 31}));
  ASSERT_EQ(0, persons.count(Person{"Anne", 32}));
  ASSERT_EQ(1, persons.count(Person{"John", 32}));
  ASSERT_EQ(1, persons.count(Person{"Anne", 45}));
}
