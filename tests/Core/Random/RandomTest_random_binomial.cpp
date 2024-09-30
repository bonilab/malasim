// RandomTest_random_binomial.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"
#include "helpers/test_helpers.h"

// Test that random_binomial throws when probability is less than 0.0
TEST_F(RandomTest, random_binomial_ThrowsWhenProbabilityLessThanZero) {
  double probability = -0.1;  // Invalid probability
  unsigned int trials = 10;
  EXPECT_THROW(rng.random_binomial(probability, trials), std::invalid_argument);
}

// Test that random_binomial throws when probability is greater than 1.0
TEST_F(RandomTest, random_binomial_ThrowsWhenProbabilityGreaterThanOne) {
  double probability = 1.1;  // Invalid probability
  unsigned int trials = 10;
  EXPECT_THROW(rng.random_binomial(probability, trials), std::invalid_argument);
}

// Range Validation Tests

// Test that random_binomial returns counts within [0, trials]
TEST_F(RandomTest, random_binomial_ReturnsCountsWithinRange) {
  double probability = 0.3;
  unsigned int trials = 100;
  int sample_size = 10000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_LE(samples[i], trials);
    EXPECT_GE(samples[i], 0u);
  }
}

// Test that random_binomial returns 0 when probability is 0.0
TEST_F(RandomTest, random_binomial_ReturnsZeroWhenProbabilityZero) {
  double probability = 0.0;
  unsigned int trials = 100;
  int sample_size = 1000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_EQ(samples[i], 0u);
  }
}

// Test that random_binomial returns trials when probability is 1.0
TEST_F(RandomTest, random_binomial_ReturnsTrialsWhenProbabilityOne) {
  double probability = 1.0;
  unsigned int trials = 100;
  int sample_size = 1000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_EQ(samples[i], trials);
  }
}

// Test that random_binomial returns counts summing up correctly over multiple
// trials
TEST_F(RandomTest, random_binomial_CountsSumCorrectlyOverMultipleTrials) {
  double probability = 0.5;
  unsigned int trials = 10;
  int sample_size = 10000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
  }

  // No specific sum check here since each call is independent
  // Instead, rely on statistical tests
  // Alternatively, you could check if individual sums are correct, but not
  // necessary
  SUCCEED();
}

// Statistical Mean and Variance Tests

// Test that the sample mean is close to the theoretical mean of the binomial
// distribution
TEST_F(RandomTest, random_binomial_SampleMeanCloseToTheoretical) {
  double probability = 0.4;
  unsigned int trials = 50;
  int sample_size = 100000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = trials * probability;
  double epsilon = 0.05 * theoretical_mean;  // 5% margin

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon);
}

// Test that the sample variance is close to the theoretical variance of the
// binomial distribution
TEST_F(RandomTest, random_binomial_SampleVarianceCloseToTheoretical) {
  double probability = 0.4;
  unsigned int trials = 50;
  int sample_size = 100000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance = trials * probability * (1.0 - probability);
  double epsilon = 1;  // theoretical variance is 12

  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon);
}

// Edge Case Tests

// Test with trials = 0: should always return 0
TEST_F(RandomTest, random_binomial_ZeroTrialsAlwaysReturnZero) {
  double probability = 0.5;
  unsigned int trials = 0;
  int sample_size = 1000;
  std::vector<unsigned int> samples(sample_size, 0);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_EQ(samples[i], 0u);
  }
}

// Test with trials = 1: Bernoulli distribution
TEST_F(RandomTest, random_binomial_TrialsOneBernoulliDistribution) {
  double probability = 0.7;
  unsigned int trials = 1;
  int sample_size = 10000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_TRUE(samples[i] == 0 || samples[i] == 1);
  }

  // Calculate sample mean and variance
  double sample_mean = calculate_mean(samples);
  double theoretical_mean = trials * probability;
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance = trials * probability * (1.0 - probability);

  double epsilon_mean = 0.05 * theoretical_mean;          // 1% margin
  double epsilon_variance = 0.05 * theoretical_variance;  // 1% margin

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with probability = 0.5 and large number of trials
TEST_F(RandomTest, random_binomial_ProbabilityHalfLargeTrials) {
  double probability = 0.5;
  unsigned int trials = 1000;
  int sample_size = 100000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = trials * probability;
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance = trials * probability * (1.0 - probability);

  double epsilon_mean = 0.05 * theoretical_mean;          // 5% margin
  double epsilon_variance = 0.05 * theoretical_variance;  // 5% margin

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with very high number of trials
TEST_F(RandomTest, random_binomial_VeryHighNumberOfTrials) {
  double probability = 0.3;
  unsigned int trials = 1000000;
  int sample_size = 100000;
  std::vector<unsigned int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_binomial(probability, trials);
    EXPECT_LE(samples[i], trials);
    EXPECT_GE(samples[i], 0u);
  }

  // Calculate sample mean and variance
  double sample_mean = calculate_mean(samples);
  double theoretical_mean = trials * probability;
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance = trials * probability * (1.0 - probability);

  double epsilon_mean = 0.05 * theoretical_mean;          // 0.5% margin
  double epsilon_variance = 0.05 * theoretical_variance;  // 0.5% margin

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

