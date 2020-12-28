#if __has_include(<experimental/propagate_const>)

#include <experimental/propagate_const>

class Foo {
  std::experimental::propagate_const<int*> ptr_ = nullptr;
  int val_{};
public:
  auto set_ptr(int* p) const {
    // ptr_ = p; // Will not compile, as expected
  }
  auto set_val(int v) const {
    // val_ = v; // Will not compile, as expected
  }
  auto set_ptr_val(int v) const {
    // *ptr_ = v; // Will not compile, const is propagated
  }
};

#endif // experimental/propagate_const
