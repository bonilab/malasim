#ifndef INDEXER_H
#define INDEXER_H
#include <cstdint>

namespace utils{
  class Indexer {
    // Disable copy and assignment
    Indexer(const Indexer&) = delete;
    Indexer& operator=(const Indexer&) = delete;
    Indexer(Indexer&&) = delete;
    Indexer& operator=(Indexer&&) = delete;

  public:
    int64_t index;

    Indexer() : index(-1) {}

    ~Indexer() = default;

  };
}

#endif