#ifndef INDEXER_H
#define INDEXER_H

#include <cstddef>
namespace utils {
class Indexer {
public:
  // Disable copy and assignment
  Indexer(const Indexer &) = delete;
  void operator=(const Indexer &) = delete;
  Indexer(Indexer &&) = delete;
  Indexer &operator=(Indexer &&) = delete;

  Indexer() = default;

  virtual ~Indexer() = default;

  void set_index(const size_t &value) { index_ = value; }
  [[nodiscard]] size_t get_index() const { return index_; }

private:
  size_t index_{};
};
}  // namespace utils

#endif

