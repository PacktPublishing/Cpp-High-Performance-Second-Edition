#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <exception>
#include <utility>

template <typename T>
struct Generator {

private:
  struct Promise {
  T value_;
  auto get_return_object() -> Generator {
    using Handle = std::coroutine_handle<Promise>;
    return Generator{Handle::from_promise(*this)};
  }
  auto initial_suspend() { return std::suspend_always{}; }
  auto final_suspend() noexcept { return std::suspend_always{}; }
  void return_void() {}
  void unhandled_exception() { throw; }
  auto yield_value(T&& value) {
    value_ = std::move(value);
    return std::suspend_always{};
  }
   auto yield_value(const T& value) {
    value_ = value;
    return std::suspend_always{};
  }
};

struct Sentinel {};

struct Iterator {

  using iterator_category = std::input_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  std::coroutine_handle<Promise> h_;
  Iterator& operator++() {
    h_.resume();
    return *this;
  }
  void operator++(int) { (void)operator++(); }
  T operator*() const { return  h_.promise().value_; }
  pointer operator->() const { return std::addressof(operator*()); }
  bool operator==(Sentinel) const { return h_.done(); }
};

  
  std::coroutine_handle<Promise> h_;
  explicit Generator(std::coroutine_handle<Promise> h) : h_(h) {}

public: 
  using promise_type = Promise;

  Generator(Generator&& g) : h_(std::exchange(g.h_, {})) {}
  ~Generator() { if (h_) { h_.destroy();  } }

  auto begin() {
    h_.resume();
    return Iterator{h_};
  }
  auto end() { return Sentinel{}; }
};

#endif // SUPPORTS_COROUTINES