#include <iostream>
#include <forward_list>
#include <queue>
#include <string>
#include <vector>
#include <memory>
#include <gtest/gtest.h>

//
// This example demonstrates how to use a priority queue for
// partially sorting a list which only provides forward iterators.
//
namespace {

class Document {
public:
  Document(const std::string& title)
  : title_{title}
  {}
  auto title() const {
    return title_;
  }
private:
  std::string title_;
};

struct Hit {
  float rank_{};
  std::shared_ptr<Document> document_;
};

template<typename It>
auto sort_hits(It begin, It end, size_t m) -> std::vector<Hit> {
  auto cmp = [](const Hit& a, const Hit& b) {
    return a.rank_ > b.rank_; // Note, we are using greater than
  };
  auto queue = std::priority_queue<Hit, std::vector<Hit>, decltype(cmp)>{cmp};

  for (auto it = begin; it != end; ++it) {
    if (queue.size() < m) {
      queue.push(*it);
    }
    else if (it->rank_ > queue.top().rank_) {
      queue.pop();
      queue.push(*it);
    }
  }

  auto result = std::vector<Hit>{};
  while (!queue.empty()) {
    result.push_back(queue.top());
    queue.pop();
  }
  std::reverse(result.begin(), result.end());
  return result;
}

// Utility to create some test data
auto generate_random_hit(const std::string& title) {
  auto hit = Hit{};
  hit.document_ = std::make_shared<Document>(title);
  hit.rank_ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  return hit;
}

} // namespace

TEST(PriorityQueues, Top10Hits) {
  auto hits = std::forward_list<Hit>{};
  for (char c = 'a'; c <= 'z'; ++c) {
    hits.emplace_front(generate_random_hit(std::string{c}));
  }
  auto top_ten_hits = sort_hits(hits.begin(), hits.end(), 10);
  for (const auto& hit : top_ten_hits) {
    std::cout << hit.rank_ << " " << hit.document_->title() << '\n';
  }

  // Check order of hints
  for (size_t i = 1; i < top_ten_hits.size(); ++i) {
    ASSERT_TRUE(top_ten_hits[i-1].rank_ > top_ten_hits[i].rank_);
  }
}
