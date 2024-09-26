
#include <gtest/gtest.h>

#include "RandomTestBase.h"

// Test default constructor initializes RNG
TEST_F(RandomTest, DefaultConstructorInitializesRNG) {
  Random const rng;
  EXPECT_NE(rng.get_seed(), 0);
}

// Test constructor with external RNG
TEST_F(RandomTest, ConstructorWithExternalRNG) {
  gsl_rng* external_rng = gsl_rng_alloc(gsl_rng_mt19937);
  ASSERT_NE(external_rng, nullptr);

  gsl_rng_set(external_rng, 12345);  // Set a known seed
  Random rng(external_rng, 12345);
  EXPECT_EQ(rng.get_seed(), 12345);

  // gsl_rng should be owned by Random, so we don't free it here
}

// Test get_seed and set_seed
TEST_F(RandomTest, SeedManagement) {
  uint64_t initial_seed = rng.get_seed();
  EXPECT_NE(initial_seed, 0);

  rng.set_seed(67890);
  EXPECT_EQ(rng.get_seed(), 67890);

  // Reset seed and check
  rng.set_seed(123456789);
  EXPECT_EQ(rng.get_seed(), 123456789);
}
// Test constructor with nullptr (should initialize internally)
TEST_F(RandomTest, ConstructorWithNullptr) {
  Random rng_instance(nullptr);
  EXPECT_NE(rng_instance.get_seed(), 0);
}

