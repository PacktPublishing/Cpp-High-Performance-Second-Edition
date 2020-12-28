#include "chapter_13.h"
#ifdef SUPPORTS_COROUTINES

#include <gtest/gtest.h>

#include "sync_wait.h"
#include "task.h"

#include <iostream>


namespace {

Task<int> height() { co_return 20; }
Task<int> width() { co_return 30; }
Task<int> area() {
  auto h = co_await height();
  auto w = co_await width();
  co_return h* w;
}

} // namespace

TEST(TaskEx, Area) {
  auto a = area();
  auto x = sync_wait(a);
  std::cout << x << std::endl;
}

#endif // SUPPORTS_COROUTINES