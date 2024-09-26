// RandomTest_random_beta.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"

// Test that random_beta throws when alpha is non-positive
TEST_F(RandomTest, random_beta_ThrowsWhenAlphaNonPositive) {
  double alpha = -1.0;  // Invalid alpha
  double beta = 2.0;
  EXPECT_THROW(rng.random_beta(alpha, beta), std::invalid_argument);
}

// Test that random_beta throws when beta is negative (assuming gsl_ran_beta
// does not accept it)
TEST_F(RandomTest, random_beta_ThrowsWhenBetaNegative) {
  double alpha = 2.0;
  double beta = -3.0;  // Invalid beta
  EXPECT_THROW(rng.random_beta(alpha, beta), std::invalid_argument);
}

// Range Validation Tests

// Test that random_beta returns alpha when beta is zero
TEST_F(RandomTest, random_beta_ReturnsAlphaWhenBetaZero) {
  double alpha = 0.7;
  double beta = 0.0;
  double result = rng.random_beta(alpha, beta);
  EXPECT_DOUBLE_EQ(result, alpha);
}

// Test that random_beta returns values within [0, 1) when beta is not zero
TEST_F(RandomTest, random_beta_ReturnsValuesWithinRange) {
  double alpha = 2.0;
  double beta = 5.0;
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    double val = rng.random_beta(alpha, beta);
    EXPECT_GE(val, 0.0);
    EXPECT_LT(val, 1.0);
  }
}

// Statistical Mean and Variance Tests

// Test that the sample mean is close to the theoretical mean of the Beta
// distribution
TEST_F(RandomTest, random_beta_SampleMeanCloseToTheoretical) {
  double alpha = 2.0;
  double beta = 5.0;
  int sample_size = 100000;
  double epsilon = 0.01;  // Acceptable error margin

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = alpha / (alpha + beta);

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon);
}

// Test that the sample variance is close to the theoretical variance of the
// Beta distribution
TEST_F(RandomTest, random_beta_SampleVarianceCloseToTheoretical) {
  double alpha = 2.0;
  double beta = 5.0;
  int sample_size = 100000;
  double epsilon = 0.01;  // Acceptable error margin

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);

  double theoretical_variance =
      (alpha * beta) / (pow(alpha + beta, 2) * (alpha + beta + 1));

  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon);
}

// Edge Case Tests

// Test with alpha and beta equal to 1 (Uniform distribution)
TEST_F(RandomTest, random_beta_BetaAlpha1Beta1_UniformDistribution) {
  double alpha = 1.0;
  double beta = 1.0;
  int sample_size = 100000;
  double epsilon = 0.01;

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  // Sample mean should be 0.5
  double sample_mean = calculate_mean(samples);
  EXPECT_NEAR(sample_mean, 0.5, epsilon);

  // Sample variance should be 1/12 ≈ 0.0833
  double sample_variance = calculate_variance(samples, sample_mean);
  EXPECT_NEAR(sample_variance, 1.0 / 12.0, epsilon);
}

// Test with large alpha and beta values
TEST_F(RandomTest, random_beta_LargeAlphaBetaValues) {
  double alpha = 50.0;
  double beta = 50.0;
  int sample_size = 100000;
  double epsilon_mean = 0.1;
  double epsilon_variance = 0.1;

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = alpha / (alpha + beta);
  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);

  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance =
      (alpha * beta) / (pow(alpha + beta, 2) * (alpha + beta + 1));
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with alpha is much larger than beta
TEST_F(RandomTest, random_beta_AlphaMuchLargerThanBeta) {
  double alpha = 10.0;
  double beta = 1.0;
  int sample_size = 100000;
  double epsilon_mean = 0.01;
  double epsilon_variance = 0.01;

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = alpha / (alpha + beta);
  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);

  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance =
      (alpha * beta) / (pow(alpha + beta, 2) * (alpha + beta + 1));
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with beta is much larger than alpha
TEST_F(RandomTest, random_beta_BetaMuchLargerThanAlpha) {
  double alpha = 1.0;
  double beta = 10.0;
  int sample_size = 100000;
  double epsilon_mean = 0.01;
  double epsilon_variance = 0.01;

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_beta(alpha, beta);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = alpha / (alpha + beta);
  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);

  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance =
      (alpha * beta) / (pow(alpha + beta, 2) * (alpha + beta + 1));
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}
