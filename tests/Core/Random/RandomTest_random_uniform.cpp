// RandomTest_random_uniform.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"

// Test that random_uniform returns values within [0, upper_bound)
TEST_F(RandomTest, random_uniform_ReturnsValuesWithinRange) {
  uint64_t upper_bound = 10;
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    uint64_t val = rng.random_uniform(upper_bound);
    EXPECT_GE(val, 0);
    EXPECT_LT(val, upper_bound);
  }
}

// Test that random_uniform() returns values within [0, 1)
TEST_F(RandomTest, random_uniform_nobound_ReturnsValuesWithinRange) {
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    double val = rng.random_uniform();
    EXPECT_GE(val, 0.0);
    EXPECT_LT(val, 1.0);
  }
}

// Test that the sample mean is close to the expected mean (0.5)
TEST_F(RandomTest, random_uniform_nobound_SampleMeanCloseToExpected) {
  int sample_size = 100000;
  double epsilon = 0.05;  // Acceptable error margin

  double sum = 0.0;
  for (int i = 0; i < sample_size; ++i) { sum += rng.random_uniform(); }
  double sample_mean = sum / sample_size;
  double expected_mean = 0.5;

  EXPECT_NEAR(sample_mean, expected_mean, epsilon);
}

// Test that random_uniform throws when from >= to (Integral Type)
TEST_F(RandomTest,
       random_uniform_range_ThrowsWhenFromGreaterThanOrEqualToTo_Integral) {
  int from = 20;
  int to = 10;
  EXPECT_THROW(rng.random_uniform<int>(from, to), std::invalid_argument);

  from = 10;
  to = 10;
  EXPECT_THROW(rng.random_uniform<int>(from, to), std::invalid_argument);
}

// Test that random_uniform throws when from >= to (Floating-Point Type)
TEST_F(RandomTest,
       random_uniform_range_ThrowsWhenFromGreaterThanOrEqualToTo_Floating) {
  double from = 1.0;
  double to = 0.0;
  EXPECT_THROW(rng.random_uniform<double>(from, to), std::invalid_argument);

  from = 1.0;
  to = 1.0;
  EXPECT_THROW(rng.random_uniform<double>(from, to), std::invalid_argument);
}

// Integral Type Tests

// Test that random_uniform returns values within [from, to) for integral types
TEST_F(RandomTest, random_uniform_range_ReturnsValuesWithinRange_Integral) {
  int from = 10;
  int to = 20;
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    int val = rng.random_uniform<int>(from, to);
    EXPECT_GE(val, from);
    EXPECT_LT(val, to);
  }
}

// Test that the sample mean is close to the expected mean for integral types
TEST_F(RandomTest, random_uniform_range_SampleMeanCloseToExpected_Integral) {
  int from = 10;
  int to = 20;
  int sample_size = 100000;
  double epsilon = 0.1;  // Acceptable error margin

  double sum = 0.0;
  for (int i = 0; i < sample_size; ++i) {
    sum += static_cast<double>(rng.random_uniform<int>(from, to));
  }
  double sample_mean = sum / sample_size;
  double expected_mean =
      (static_cast<double>(from) + static_cast<double>(to - 1)) / 2.0;

  EXPECT_NEAR(sample_mean, expected_mean, epsilon);
}

// Floating-Point Type Tests

// Test that random_uniform returns values within [from, to) for floating-point
// types
TEST_F(RandomTest, random_uniform_range_ReturnsValuesWithinRange_Floating) {
  double from = 0.0;
  double to = 1.0;
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    double val = rng.random_uniform<double>(from, to);
    EXPECT_GE(val, from);
    EXPECT_LT(val, to);
  }
}

// Test that the sample mean is close to the expected mean for floating-point
// types
TEST_F(RandomTest, random_uniform_range_SampleMeanCloseToExpected_Floating) {
  double from = 0.0;
  double to = 1.0;
  int sample_size = 100000;
  double epsilon = 0.05;  // Acceptable error margin

  double sum = 0.0;
  for (int i = 0; i < sample_size; ++i) {
    sum += rng.random_uniform<double>(from, to);
  }
  double sample_mean = sum / sample_size;
  double expected_mean = (from + to) / 2.0;

  EXPECT_NEAR(sample_mean, expected_mean, epsilon);
}

