#include "chapter_12.h"
#ifdef SUPPORTS_COROUTINES

#include <gtest/gtest.h>

#include "resumable.h"

#include <limits>

auto coroutine() -> Resumable {            // Initial suspend
  std::cout << "3 ";
  co_await std::suspend_always{};  // Suspend (explicit)
  std::cout << "5 ";
}                                  


TEST(ResumableMinimalExample, ResumeCoroutine) {
  std::cout << "1 ";
  auto resumable = coroutine(); // Create coroutine state
  std::cout << "2 ";
  resumable.resume();           // Resume
  std::cout << "4 ";
  resumable.resume();           // Resume
  std::cout << "6 ";
}                               // Destroy coroutine state
// Prints: 1 2 3 4 5 6

#endif // SUPPORTS_COROUTINES