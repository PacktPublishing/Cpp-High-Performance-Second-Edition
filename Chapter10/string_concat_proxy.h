#pragma once

#include <string>

auto is_concat_equal(const std::string& a, const std::string& b,
                     const std::string& c) {
  return a.size() + b.size() == c.size() &&
         std::equal(a.begin(), a.end(), c.begin()) &&
         std::equal(b.begin(), b.end(), c.begin() + a.size());
}

class String {
public:
  String() = default;
  String(std::string istr) : str_{std::move(istr)} {}
  std::string str_{};
};

struct ConcatProxy {
  const std::string& a;
  const std::string& b;
  operator String() const&& { return String{a + b}; }
};

auto operator+(const String& a, const String& b) {
  return ConcatProxy{a.str_, b.str_};
}

auto operator==(ConcatProxy&& concat, const String& str) -> bool {
  return is_concat_equal(concat.a, concat.b, str.str_);
}