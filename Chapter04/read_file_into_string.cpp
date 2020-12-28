#include <string>
#include <fstream>
#include <gtest/gtest.h>

//
// This example demonstrates how to read an entire text file
// into an std::string.
//

class ReadFileIntoString : public testing::Test {
protected:

  // This method will be executed before test start
  void SetUp() override {
    // Create file that will be read in text
    initial_content_ = std::string{"Hello, world!"};
    auto out = std::ofstream{"file.txt"};
    out.write(initial_content_.data(), initial_content_.size());
  }

  std::string initial_content_{};
};

TEST_F(ReadFileIntoString, CheckFileSize) {
  auto in = std::ifstream{"file.txt", std::ios::binary | std::ios::ate};
  ASSERT_TRUE(in);

  const auto size = static_cast<size_t>(in.tellg());
  auto content = std::string(size, '\0');
  in.seekg(0);
  in.read(&content[0], size);
  // contents now contains the entire file

  ASSERT_EQ(initial_content_, content);
  ASSERT_EQ(static_cast<size_t>(size), initial_content_.size());
}
