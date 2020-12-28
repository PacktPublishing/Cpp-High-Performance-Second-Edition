#pragma once

#include <atomic>
#include <cassert>
#include <future>
#include <vector>

//
// Sequential copy_if() similar to std::copy_if()
//
template <typename SrcIt, typename DstIt, typename Pred>
auto copy_if(SrcIt first, SrcIt last, DstIt dst, Pred pred) -> DstIt {
  for (auto it = first; it != last; ++it) {
    if (pred(*it)) {
      *dst = *it;
      ++dst;
    }
  }
  return dst;
}


// 
// par_copy_if_sync()
// Parallel copy_if using an atomic index
//

template <typename SrcIt, typename DstIt, typename Pred>
auto inner_par_copy_if_sync(SrcIt first, SrcIt last, DstIt dst,
                            std::atomic_size_t& dst_idx, Pred pred,
                            size_t chunk_sz) {
  auto n = static_cast<size_t>(std::distance(first, last));
  if (n <= chunk_sz) {
    std::for_each(first, last, [&](const auto& ival) {
      if (pred(ival)) {
        const auto write_idx = dst_idx.fetch_add(1);
        *std::next(dst, write_idx) = ival;
      }
    });
    return;
  }
  auto middle = std::next(first, n / 2);
  auto future = std::async([first, middle, dst, chunk_sz, &pred, &dst_idx] {
    return inner_par_copy_if_sync(first, middle, dst, dst_idx, pred, chunk_sz);
  });
  inner_par_copy_if_sync(middle, last, dst, dst_idx, pred, chunk_sz);
  future.wait();
}

template <typename SrcIt, typename DstIt, typename Pred>
auto par_copy_if_sync(SrcIt first, SrcIt last, DstIt dst, Pred pred,
                      size_t chunk_sz) {
  auto&& dst_write_idx = std::atomic_size_t{0};
  inner_par_copy_if_sync(first, last, dst, dst_write_idx, pred, chunk_sz);
  return std::next(dst, dst_write_idx);
}


//
// par_copy_if_split()
// Parallel copy_if() writing to a sparse range, then squeeze
//
template <typename SrcIt, typename DstIt, typename Pred>
auto par_copy_if_split(SrcIt first, SrcIt last, DstIt dst, Pred pred,
                       size_t chunk_sz) -> DstIt {
  auto n = static_cast<size_t>(std::distance(first, last));
  auto futures = std::vector<std::future<std::pair<DstIt, DstIt>>>{};
  futures.reserve(n / chunk_sz);

  for (size_t i = 0; i < n; i += chunk_sz) {
    const auto stop_idx = std::min(i + chunk_sz, n);
    auto future = std::async([=, &pred] {
      auto dst_first = dst + i;
      auto dst_last =
          std::copy_if(first + i, first + stop_idx, dst_first, pred);
      return std::make_pair(dst_first, dst_last);
    });
    futures.emplace_back(std::move(future));
  }

  auto new_end = futures.front().get().second;
  for (auto it = std::next(futures.begin()); it != futures.end(); ++it) {
    auto chunk_range = it->get();
    new_end = std::move(chunk_range.first, chunk_range.second, new_end);
  }
  return new_end;
}