#pragma once


template <size_t N> class Arena {

  static constexpr size_t alignment = alignof(std::max_align_t);

public:
  Arena() noexcept : ptr_(buffer_) {}
  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  auto reset() noexcept { ptr_ = buffer_; }
  static constexpr auto size() noexcept { return N; }
  auto used() const noexcept { return static_cast<size_t>(ptr_ - buffer_); }
  auto allocate(size_t n) -> std::byte*;
  auto deallocate(std::byte* p, size_t n) noexcept -> void;

private:
  static auto align_up(size_t n) noexcept -> size_t {
    return (n + (alignment - 1)) & ~(alignment - 1);
  }
  auto pointer_in_buffer(const std::byte* p) const noexcept -> bool {
    return std::uintptr_t(buffer_) <= std::uintptr_t(p) &&
           std::uintptr_t(p) < std::uintptr_t(buffer_) + N;
  }
  alignas(alignment) std::byte buffer_[N];
  std::byte* ptr_{};
};

template <size_t N> auto Arena<N>::allocate(size_t n) -> std::byte* {
  const auto aligned_n = align_up(n);
  const auto available_bytes =
      static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
  if (available_bytes >= aligned_n) {
    auto* r = ptr_;
    ptr_ += aligned_n;
    return r;
  }
  return static_cast<std::byte*>(::operator new(n));
}

template <size_t N>
auto Arena<N>::deallocate(std::byte* p, size_t n) noexcept -> void {
  if (pointer_in_buffer(p)) {
    n = align_up(n);
    if (p + n == ptr_) {
      ptr_ = p;
    }
  } else {
    ::operator delete(p);
  }
}
