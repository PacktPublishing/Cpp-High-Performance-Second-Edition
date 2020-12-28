#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <version>

#include <gtest/gtest.h>

#include "resumable.h"

#include <functional>
#include <iostream>
#include <limits>
#include <string>

namespace {

// Warning: Dangling reference to str!
auto coroutine_ref(const std::string& str) -> Resumable { // Warning! Reference
  std::cout << str;
  co_return;
}

auto coro_factory_ref() {
  auto str = std::string{"ABC"};
  auto res = coroutine_ref(str);
  return res;
}

// OK: Pass string by value
auto coroutine(std::string str) -> Resumable { // OK, by value!
  std::cout << str;
  co_return;
}
auto coro_factory() {
  auto str = std::string{"ABC"};
  auto res = coroutine(str);
  return res;
}

// Warning: Dangling reference to str!
auto lambda_factory() {
  auto str = std::string{"ABC"};
  auto lambda = [&str]() { // Capture str by reference
    std::cout << str;
  };
  return lambda; // Ops! str in lambda becomes
} // a dangling reference

} // namespace

TEST(DanglingReferences, CoroutineWithStringReference) {
  auto coro = coro_factory_ref();
  // coro.resume(); // Undefined behavior
}

TEST(DanglingReferences, CoroutineWithStringValue) {
  auto coro = coro_factory();
  coro.resume(); // OK!
}

TEST(DanglingReferences, LambdaWithStringReference) {
  auto f = lambda_factory();
  // f(); // Undefined behavior
}

//
// Coroutines that are member functions
//

namespace {

struct Widget {
  auto coroutine() -> Resumable {    // A member function
    std::cout << i++ << " "; // Access data member
    co_await std::suspend_always{};
    std::cout << i++ << " ";
  }
  int i{};
};

auto widget_coro_factory() { // Create and return a coroutine
  auto w = Widget{};
  auto coro = w.coroutine();
  return coro;
} // Warning: Object w destructs here

} // namespace

TEST(DanglingReferences, WidgetMemberFunction) {
  auto w = Widget{99};
  auto coro = w.coroutine();
  coro.resume();
  coro.resume();
  // Prints: 99 100
}

TEST(DanglingReferences, WidgetMemberFunctionUndefinedBehavior) {
  auto r = widget_coro_factory();
  // r.resume(); // Undefined behavior
  // r.resume();
}

//
// Lambdas that are coroutines
//

TEST(DanglingReferences, CoroutineLambdaOK) {

  auto lambda = [](int i) -> Resumable {
    std::cout << i;
    co_return; // Make it a coroutine
  };
  auto coro = lambda(42); // Call, creates the coroutine frame
  coro.resume();          // Outputs: 42
}

TEST(DanglingReferences, CoroutineLambdaUndefinedBehavior) {
  auto coro = [i = 0]() mutable -> Resumable {
    std::cout << i++;
    co_await std::suspend_always{};
    std::cout << i++;
  }(); // Invoke lambda immediately
  // coro.resume(); // Undefined behavior! Function object
  // coro.resume(); // already destructed
}

#endif // SUPPORTS_COROUINES