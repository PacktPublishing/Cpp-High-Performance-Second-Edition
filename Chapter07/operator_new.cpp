#include <iostream>
#include <gtest/gtest.h>

auto allocated = size_t{0};
auto print_allocation = bool{false};

void* operator new(size_t isize) {
  void* p = std::malloc(isize);
  if (print_allocation) {
    std::cout << "allocated " << isize << " byte(s)\n";
  }
  allocated += isize;
  return p;
}

void operator delete(void* p) noexcept {
  if (print_allocation) {
    std::cout << "deleted memory\n";
  }
  std::free(p);
}

void operator delete(void* p, std::size_t) noexcept {
  operator delete(p);
}

auto operator new[](size_t isize) -> void* {
  void* p = std::malloc(isize);
  if (print_allocation) {
    std::cout << "allocated " << isize << " byte(s) with new[]\n";
  }
  return p;
}

void operator delete[](void* p) noexcept {
  if (print_allocation) {
    std::cout << "deleted memory with delete[]\n";
  }
  std::free(p);
}

void operator delete [](void* p, std::size_t) noexcept { 
  operator delete[](p);
}

class Document {

private:
  double rank_{};
  int id_{};
  bool is_cached_{};

public:
  auto operator new(size_t size) -> void* {
    return ::operator new(size);
  }
  auto operator delete(void* p) -> void {
    ::operator delete(p);
  }
};

class OperatorNew : public ::testing::Test {
protected:
  void SetUp() override {
    print_allocation = true;
  }

  void TearDown() override {
    print_allocation = false;
  }
};

TEST_F(OperatorNew, AllocateAndDeleteMemory) {
  {
    auto* p = new char{'a'}; // Uses global operator new
    delete p;
  }
  {
    auto* p = new char[10]; // Use operator new[]
    delete[] p;
  }
  {
    auto* p = new Document{}; // Uses class specific operator new
    delete p;
  }
  {
    auto* p = ::new Document{}; // Uses global operator new
    ::delete p;
  }
}
