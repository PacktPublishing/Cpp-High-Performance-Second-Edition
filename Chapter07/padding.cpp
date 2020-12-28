#if _MSC_VER
  #pragma warning(disable : 4127) // conditional expression is constant
#endif

#include <cstdint>
#include <iostream>
#include <gtest/gtest.h>

class DocumentV1 {
  bool is_cached_{};
  double rank_{};
  int id_{};
};

class DocumentV2 {
  double rank_{};
  int id_{};
  bool is_cached_{};
};

TEST(Padding, ClassSize) {
  std::cout << sizeof(DocumentV1) << '\n'; // Possible output is 24
  std::cout << sizeof(DocumentV2) << '\n'; // Possible output is 16

  if (sizeof(double) > sizeof(int)) {
    ASSERT_TRUE(sizeof(DocumentV1) > sizeof(DocumentV2));
  }
  else {
    ASSERT_TRUE(sizeof(DocumentV1) == sizeof(DocumentV2));
  }
}
