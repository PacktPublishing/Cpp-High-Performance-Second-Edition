#pragma once

#include <iostream>
#include <chrono>

// ScopedTimer from Chapter 3
class ScopedTimer {

public:
    using ClockType = std::chrono::steady_clock;

  ScopedTimer(const char* func) :
    function_{func}, start_{ClockType::now()} {
  }

  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&) = delete;
  auto operator=(const ScopedTimer&) -> ScopedTimer& = delete;
  auto operator=(ScopedTimer&&) -> ScopedTimer& = delete;

  ~ScopedTimer() {
    using namespace std::chrono;
    auto stop = ClockType::now();
    auto duration = (stop - start_);
//      auto micro = duration_cast<microseconds>(duration).count();
//      std::cout << micro << " micro sec " << function_ <<  '\n';
      auto ms = duration_cast<milliseconds>(duration).count();
      std::cout << ms << " ms " << function_ <<  '\n';
  }

private:
  const char* function_ = {};
  const ClockType::time_point start_ = {};
};
