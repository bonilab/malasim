#ifndef INDEXER_H
#define INDEXER_H
#include <cstdint>

namespace utils{
  class Indexer {
    // Disable copy and assignment
    Indexer(const Indexer&) = delete;
    void operator=(const Indexer&) = delete;
    Indexer(Indexer&&) = delete;
    Indexer& operator=(Indexer&&) = delete;

  public:
    int64_t index;

    Indexer() : index(-1) {}

    ~Indexer() = default;

    void set_index(const int64_t &value) {
      index = value;
    }
    [[nodiscard]] int64_t get_index() const {
      return index;
    }

  };
}

#endif