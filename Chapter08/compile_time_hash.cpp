#if _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <unordered_map>

constexpr auto hash_function(const char* str) -> size_t {
  auto sum = size_t{0};
  for (auto ptr = str; *ptr != '\0'; ++ptr)
    sum += *ptr;
  return sum;
}

class PrehashedString {
public:
  template <size_t N>
  constexpr PrehashedString(const char (&str)[N])
      : hash_{hash_function(&str[0])},
        size_{N - 1}, // The subtraction is to avoid null at end
        strptr_{&str[0]} {}
  auto operator==(const PrehashedString& s) const {

    return size_ == s.size_ && std::equal(c_str(), c_str() + size_, s.c_str());
  }
  auto operator!=(const PrehashedString& s) const { return !(*this == s); }
  constexpr auto size() const { return size_; }
  constexpr auto get_hash() const { return hash_; }
  constexpr auto c_str() const -> const char* { return strptr_; }

private:
  size_t hash_{};
  size_t size_{};
  const char* strptr_{nullptr};
};
namespace std {
template <>
struct hash<PrehashedString> {
  constexpr auto operator()(const PrehashedString& s) const {
    return s.get_hash();
  }
};
} // namespace std

struct Bitmap {};

// External function which loads a bitmap from the filesystem
auto load_bitmap_from_filesystem(const char * /*path*/) -> Bitmap {
  // ...
  return Bitmap{};
}

// Bitmap cache
auto get_bitmap_resource(const PrehashedString& path) -> const Bitmap& {
  // Static storage of all loaded bitmaps
  static auto loaded_bitmaps = std::unordered_map<PrehashedString, Bitmap>{};
  // If the bitmap is already in loaded_bitmaps, return it
  if (loaded_bitmaps.count(path) > 0) {
    return loaded_bitmaps.at(path);
  }
  // The bitmap isn’t already loaded, load and return it
  auto bitmap = load_bitmap_from_filesystem(path.c_str());
  loaded_bitmaps.emplace(path, std::move(bitmap));
  return loaded_bitmaps.at(path);
}

auto test_prehashed_string() {
  const auto& hash_fn = std::hash<PrehashedString>{};
  const auto& str = PrehashedString("abc");
  return hash_fn(str);
}

TEST(CompileTimeHash, HashOfString) {
  auto prehashed = test_prehashed_string();
  auto hash = hash_function("abc");
  ASSERT_EQ(hash, prehashed);
}
