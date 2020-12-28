#if _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <algorithm>
#include <cctype>
#include <gtest/gtest.h>
#include <initializer_list>
#include <locale>
#include <vector>

namespace {

//
// Rule of five
class Buffer {
public:
  // Constructor
  Buffer(const std::initializer_list<float>& values) : size_{values.size()} {
    ptr_ = new float[values.size()];
    std::copy(values.begin(), values.end(), ptr_);
  }

  // 1. Copy constructor
  Buffer(const Buffer& other) : size_{other.size_} {
    ptr_ = new float[size_];
    std::copy(other.ptr_, other.ptr_ + size_, ptr_);
  }

  // 2. Copy assignment
  auto& operator=(const Buffer& other) {
    delete[] ptr_;
    ptr_ = new float[other.size_];
    size_ = other.size_;
    std::copy(other.ptr_, other.ptr_ + size_, ptr_);
    return *this;
  }

  // 3. Destructor
  ~Buffer() {
    delete[] ptr_; // Note, it is valid to delete a nullptr
    ptr_ = nullptr;
  }

  // 4. Move constructor
  Buffer(Buffer&& other) noexcept : size_{other.size_}, ptr_{other.ptr_} {
    other.ptr_ = nullptr;
    other.size_ = 0;
  }
  // 5. Move assignment
  auto& operator=(Buffer&& other) noexcept {
    ptr_ = other.ptr_;
    size_ = other.size_;
    other.ptr_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  // Iterators for accessing the data
  auto begin() const { return ptr_; }

  auto end() const { return ptr_ + size_; }

private:
  size_t size_{0};
  float* ptr_{nullptr};
};

TEST(MoveSemantics, RuleOfFive) {
  // Construct
  auto b0 = Buffer({0.0f, 0.5f, 1.0f, 1.5f});
  // 1. Copy-construct
  auto b1 = b0;
  // 2. Copy-assignment as b0 is already initialized
  b0 = b1;
  // 3. When the function exits, the destructors are automatically invoked
}

//
// Named variables and rvalues
// Rule of zero
class Button {
public:
  Button() {}

  // // Copy-constructor/copy-assignment
  // Button(const Button&) = default;
  // auto operator=(const Button&) -> Button& = default;

  // // Move-constructor/move-assignment
  // Button(Button&&) noexcept = default;
  // auto operator=(Button&&) noexcept -> Button& = default;

  // // Destructor
  // ~Button() = default;

  auto set_title(const std::string& s) { title_ = s; }
  auto set_title(std::string&& s) { title_ = std::move(s); }
  std::string title_{};
};

auto get_ok() { return std::string("OK"); }

TEST(MoveSemantics, NamedVariablesAndRvalues) {
  auto button = Button{};
  // Case 1:
  {
    auto str = std::string{"OK"};
    button.set_title(str); // copy-assignment
  }
  // Case 2:
  {
    auto str = std::string{"OK"};
    button.set_title(std::move(str)); // move-assigned
  }
  // Case 3:
  {
    button.set_title(get_ok()); // move-assigned
  }
  // Case 4:
  {
    auto str = get_ok();
    button.set_title(str); // copy-assigned
  }
  // Case 5:
  {
    const auto str = get_ok();
    button.set_title(std::move(str)); // copy-assigned
  }
}

//
// A common pitfall - moving non-resources
class Menu {
public:
  Menu(const std::initializer_list<std::string>& items) : items_{items} {}
  void select(int i) { index_ = i; }
  auto selected_item() const { return index_ != -1 ? items_[index_] : ""; }
  // ...
private:
  int index_{-1}; // Selected index
  std::vector<std::string> items_;
};

TEST(MoveSemantics, MovingNonResources) {
  auto a = Menu{"New", "Open", "Close", "Save"};
  a.select(2);
  ASSERT_TRUE(a.selected_item() == "Close");
  auto b = std::move(a);
  std::cout << "after move\n";
  // Object a has been moved from but a.index_ is still 2
  // Calling a.selcted_item() will likely crash since
  // a.index_ is 2 and a.items_ has been moved
  // auto selected = a.selected_item(); // Crash
}

//
// Accept arguments by move
auto str_to_lower(std::string str) {
  for (auto& c : str)
    c = std::tolower(c);
  return str;
}

class Widget {
public:
  Widget(std::string s)     // By value
      : s_{std::move(s)} {} // Move-construct
private:
  std::string s_;
  /* … */
};

TEST(MoveSemantics, StrToLower) {
  std::cout << "tolower\n";
  auto upper = std::string{"ABC"};
  auto lower = str_to_lower(std::move(upper));
  ASSERT_TRUE(upper.empty());
  ASSERT_FALSE(lower.empty());
}

TEST(MoveSemantics, InitializingClassMembers) {
  const auto w1 = Widget{"ABC"};
  const auto s = std::string{"ABC"};
  const auto w2 = Widget{s};
}

} // namespace