// RandomTest_random_shuffle.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"
#include "helpers/test_helpers.h"

// test that random_shuffle throws when the input vector is empty
TEST_F(RandomTest, random_shuffle_ThrowsWhenInputEmpty) {
  std::vector<int> empty;
  EXPECT_THROW(rng.shuffle(empty), std::invalid_argument);
}

// Rewritten Test: Shuffle maintains all elements without using chi-squared
TEST_F(RandomTest, random_shuffle_ShuffleMaintainsAllElements_LargeVector) {
  std::size_t size = 1000;
  std::vector<int> original(size);
  // Initialize the vector with unique values
  for (std::size_t i = 0; i < size; ++i) { original[i] = static_cast<int>(i); }

  // Create a copy to shuffle
  std::vector<int> shuffled = original;

  // Perform shuffle
  rng.shuffle(shuffled);

  // Check that shuffled is a permutation of original
  EXPECT_TRUE(is_permutation_of(original, shuffled))
      << "Shuffled vector is not a permutation of the original.";

  // Optional: Check that the order has changed (high probability)
  bool order_changed = false;
  for (std::size_t i = 0; i < size; ++i) {
    if (original[i] != shuffled[i]) {
      order_changed = true;
      break;
    }
  }
  EXPECT_TRUE(order_changed) << "Shuffle did not change the order of elements.";
}

