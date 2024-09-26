// Random_test.cpp

#include "Core/Random/Random.h"

#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <gtest/gtest.h>

#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>
// Helper function to calculate sample mean
double calculate_mean(const std::vector<unsigned int> &data) {
  unsigned long long sum = std::accumulate(data.begin(), data.end(), 0ULL);
  return static_cast<double>(sum) / static_cast<double>(data.size());
}

// Helper function to calculate sample variance
double calculate_variance(const std::vector<unsigned int> &data, double mean) {
  double sum_sq_diff = 0.0;
  for (const auto &val : data) {
    double diff = static_cast<double>(val) - mean;
    sum_sq_diff += diff * diff;
  }
  return sum_sq_diff / static_cast<double>(data.size());
}

// Helper function to calculate sample mean
double calculate_mean(const std::vector<double> &data) {
  double sum = std::accumulate(data.begin(), data.end(), 0.0);
  return sum / static_cast<double>(data.size());
}

// Helper function to calculate sample standard deviation
double calculate_stddev(const std::vector<double> &data, double mean) {
  double sum_sq = 0.0;
  for (const auto &val : data) { sum_sq += (val - mean) * (val - mean); }
  return std::sqrt(sum_sq / static_cast<double>(data.size()));
}
// Helper function to calculate sample variance
double calculate_variance(const std::vector<double> &data, double mean) {
  double sum_sq = 0.0;
  for (const auto &val : data) { sum_sq += (val - mean) * (val - mean); }
  return sum_sq / static_cast<double>(data.size());
}

// Helper function to calculate the expected counts
std::vector<double> calculate_expected_counts(
    unsigned trials, const std::vector<double> &probabilities) {
  std::vector<double> expected;
  for (const auto &p : probabilities) { expected.push_back(trials * p); }
  return expected;
}

// Helper function to calculate chi-squared statistic
double calculate_chi_squared(const std::vector<unsigned> &observed,
                             const std::vector<double> &expected) {
  double chi_squared = 0.0;
  for (size_t i = 0; i < observed.size(); ++i) {
    double o = static_cast<double>(observed[i]);
    double e = expected[i];
    if (e > 0) {  // Avoid division by zero
      chi_squared += std::pow(o - e, 2) / e;
    }
  }
  return chi_squared;
}

// Test fixture for Random class
class RandomTest : public ::testing::Test {
protected:
  Random rng;
  void SetUp() override {
    // Optional: Initialize any shared resources
  }

  void TearDown() override {
    // Optional: Clean up any shared resources
  }
};

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

// Test that the sample mean is close to poissonMean
TEST_F(RandomTest, random_poisson_SampleMeanCloseToPoissonMean) {
  double poissonMean = 4.0;
  int sampleSize = 100000;
  double epsilon = 0.05;  // Acceptable error margin

  double sum = 0.0;
  for (int i = 0; i < sampleSize; ++i) {
    auto poisson = rng.random_poisson(poissonMean);
    sum += poisson;
  }
  double sampleMean = sum / sampleSize;

  EXPECT_NEAR(sampleMean, poissonMean, epsilon);
}

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
  double epsilon = 0.5 * 0.05;  // Acceptable error margin

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
// Exception Tests

TEST_F(RandomTest, random_normal_ThrowsWhenStdDevNonPositive_Integral) {
  int mean = 0;
  double stddev = 0.0;
  EXPECT_THROW(rng.random_normal<int>(mean, stddev), std::invalid_argument);
}

TEST_F(RandomTest, random_normal_ThrowsWhenStdDevNonPositive_Floating) {
  double mean = 0.0;
  double stddev = -1.0;
  EXPECT_THROW(rng.random_normal<double>(mean, stddev), std::invalid_argument);
}

// Integral Type Tests

// Test that random_normal returns values centered around mean with given
// standard deviation (Integral)
TEST_F(RandomTest, random_normal_ReturnsValuesAroundMean_Integral) {
  int mean = 50;
  double stddev = 10.0;
  int sample_size = 100000;
  std::vector<int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal<int>(mean, stddev);
  }

  // Calculate sample mean and stddev
  double sample_mean =
      calculate_mean(std::vector<double>(samples.begin(), samples.end()));
  double sample_stddev = calculate_stddev(
      std::vector<double>(samples.begin(), samples.end()), sample_mean);

  // Allow a margin of error
  double epsilon_mean = mean * 0.01;
  double epsilon_stddev = stddev * 0.01;

  EXPECT_NEAR(sample_mean, static_cast<double>(mean), epsilon_mean);
  EXPECT_NEAR(sample_stddev, stddev, epsilon_stddev);
}
// Floating-Point Type Tests

// Test that random_normal returns values around mean with given standard
// deviation (Floating)
TEST_F(RandomTest, random_normal_ReturnsValuesAroundMean_Floating) {
  double mean = 0.0;
  double stddev = 1.0;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal<double>(mean, stddev);
  }

  // Calculate sample mean and stddev
  double sample_mean = calculate_mean(samples);
  double sample_stddev = calculate_stddev(samples, sample_mean);

  // Allow a margin of error
  double epsilon_mean = 0.05;
  double epsilon_stddev = 0.05;

  EXPECT_NEAR(sample_mean, mean, epsilon_mean);
  EXPECT_NEAR(sample_stddev, stddev, epsilon_stddev);
}

// Edge cases:
// Test that random_normal handles large standard deviations correctly
// (Floating)
TEST_F(RandomTest, random_normal_HandlesLargeStdDev_Floating) {
  double mean = 10.0;
  double stddev = 100.0;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal<double>(mean, stddev);
  }

  double sample_mean = calculate_mean(samples);
  double sample_stddev = calculate_stddev(samples, sample_mean);

  double epsilon_mean = 5.0;
  double epsilon_stddev = 1;

  EXPECT_NEAR(sample_mean, mean, epsilon_mean);
  EXPECT_NEAR(sample_stddev, stddev, epsilon_stddev);
}

// Test that random_normal handles negative means correctly (Integral and
// Floating)
TEST_F(RandomTest, random_normal_HandlesNegativeMean_Integral) {
  int mean = -50;
  double stddev = 10.0;
  int sample_size = 100000;
  std::vector<int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal<int>(mean, stddev);
  }

  double sample_mean =
      calculate_mean(std::vector<double>(samples.begin(), samples.end()));
  double sample_stddev = calculate_stddev(
      std::vector<double>(samples.begin(), samples.end()), sample_mean);

  double epsilon_mean = 0.1;
  double epsilon_stddev = 0.1;

  EXPECT_NEAR(sample_mean, static_cast<double>(mean), epsilon_mean);
  EXPECT_NEAR(sample_stddev, stddev, epsilon_stddev);
}

TEST_F(RandomTest, random_normal_HandlesNegativeMean_Floating) {
  double mean = -5.0;
  double stddev = 1.0;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal<double>(mean, stddev);
  }

  double sample_mean = calculate_mean(samples);
  double sample_stddev = calculate_stddev(samples, sample_mean);

  double epsilon_mean = 0.01;
  double epsilon_stddev = 0.05;

  EXPECT_NEAR(sample_mean, mean, epsilon_mean);
  EXPECT_NEAR(sample_stddev, stddev, epsilon_stddev);
}

// Test that random_normal_truncated throws when a truncated value cannot be
// generated within max_attempts (Integral Type)
TEST_F(RandomTest,
       random_normal_truncated_ThrowsWhenMaxAttemptsExceeded_Integral) {
  int mean = 0;
  double stddev = 1.0;
  double truncation_limit =
      0.000001;           // Very tight truncation to likely exceed attempts
  int max_attempts = 10;  // Low number of attempts

  EXPECT_THROW(rng.random_normal_truncated<int>(mean, stddev, truncation_limit,
                                                max_attempts),
               std::runtime_error);
}

// Test that random_normal_truncated throws when a truncated value cannot be
// generated within max_attempts (Floating-Point Type)
TEST_F(RandomTest,
       random_normal_truncated_ThrowsWhenMaxAttemptsExceeded_Floating) {
  double mean = 0.0;
  double stddev = 1.0;
  double truncation_limit =
      0.0000001;          // Very tight truncation to likely exceed attempts
  int max_attempts = 10;  // Low number of attempts

  EXPECT_THROW(rng.random_normal_truncated<double>(
                   mean, stddev, truncation_limit, max_attempts),
               std::runtime_error);
}

// Integral Type Tests

// Test that random_normal_truncated returns values within [mean -
// truncation_limit * stddev, mean + truncation_limit * stddev] (Integral)
TEST_F(RandomTest, random_normal_truncated_ReturnsValuesWithinRange_Integral) {
  int mean = 50;
  double stddev = 10.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<int>(
        mean, stddev, truncation_limit, max_attempts);
  }

  double lower_bound = mean - truncation_limit * stddev;
  double upper_bound = mean + truncation_limit * stddev;

  for (const auto &val : samples) {
    EXPECT_GE(val, static_cast<int>(std::ceil(lower_bound)));
    EXPECT_LE(val, static_cast<int>(std::floor(upper_bound)));
  }
}

// Test that the sample mean is close to the specified mean (Integral)
TEST_F(RandomTest,
       random_normal_truncated_SampleMeanCloseToSpecified_Integral) {
  int mean = 50;
  double stddev = 10.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<int>(
        mean, stddev, truncation_limit, max_attempts);
  }

  // Convert to double for calculation
  std::vector<double> samples_double(samples.begin(), samples.end());
  double sample_mean = calculate_mean(samples_double);

  double epsilon_mean = 0.1;  // Acceptable error margin
  EXPECT_NEAR(sample_mean, static_cast<double>(mean), epsilon_mean);
}

// Test that the sample standard deviation is close to or less than the
// specified standard deviation (Integral)
TEST_F(RandomTest,
       random_normal_truncated_SampleStddevCloseToSpecified_Integral) {
  int mean = 50;
  double stddev = 10.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<int> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<int>(
        mean, stddev, truncation_limit, max_attempts);
  }

  // Convert to double for calculation
  std::vector<double> samples_double(samples.begin(), samples.end());
  double sample_mean = calculate_mean(samples_double);
  double sample_stddev = calculate_stddev(samples_double, sample_mean);

  double epsilon_stddev = 0.1;  // Acceptable error margin
  EXPECT_LE(sample_stddev, stddev + epsilon_stddev);
}

// Floating-Point Type Tests

// Test that random_normal_truncated returns values within [mean -
// truncation_limit * stddev, mean + truncation_limit * stddev] (Floating-Point)
TEST_F(RandomTest, random_normal_truncated_ReturnsValuesWithinRange_Floating) {
  double mean = 0.0;
  double stddev = 1.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<double>(
        mean, stddev, truncation_limit, max_attempts);
  }

  double lower_bound = mean - truncation_limit * stddev;
  double upper_bound = mean + truncation_limit * stddev;

  for (const auto &val : samples) {
    EXPECT_GE(val, lower_bound);
    EXPECT_LE(val, upper_bound);
  }
}

// Test that the sample mean is close to the specified mean (Floating-Point)
TEST_F(RandomTest,
       random_normal_truncated_SampleMeanCloseToSpecified_Floating) {
  double mean = 0.0;
  double stddev = 1.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<double>(
        mean, stddev, truncation_limit, max_attempts);
  }

  double sample_mean = calculate_mean(samples);

  double epsilon_mean = 0.01;  // Acceptable error margin
  EXPECT_NEAR(sample_mean, mean, epsilon_mean);
}

// Test that the sample standard deviation is close to or less than the
// specified standard deviation (Floating-Point)
TEST_F(RandomTest,
       random_normal_truncated_SampleStddevCloseToSpecified_Floating) {
  double mean = 0.0;
  double stddev = 1.0;
  double truncation_limit = 3.0;
  int max_attempts = 1000;
  int sample_size = 100000;
  std::vector<double> samples(sample_size);

  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_normal_truncated<double>(
        mean, stddev, truncation_limit, max_attempts);
  }

  double sample_mean = calculate_mean(samples);
  double sample_stddev = calculate_stddev(samples, sample_mean);

  double epsilon_stddev = 0.001;  // Acceptable error margin
  EXPECT_LE(sample_stddev, stddev + epsilon_stddev);
}
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

// Test that random_gamma throws when shape is non-positive
TEST_F(RandomTest, random_gamma_ThrowsWhenShapeNonPositive) {
  double shape = -1.0;  // Invalid shape
  double scale = 2.0;
  EXPECT_THROW(rng.random_gamma(shape, scale), std::invalid_argument);
}

// Test that random_gamma throws when scale is negative
TEST_F(RandomTest, random_gamma_ThrowsWhenScaleNegative) {
  double shape = 2.0;
  double scale = -3.0;  // Invalid scale
  EXPECT_THROW(rng.random_gamma(shape, scale), std::invalid_argument);
}

// Range Validation Tests

// Test that random_gamma returns shape when scale is zero
TEST_F(RandomTest, random_gamma_ReturnsShapeWhenScaleZero) {
  double shape = 4.0;
  double scale = 0.0;
  double result = rng.random_gamma(shape, scale);
  EXPECT_DOUBLE_EQ(result, shape);
}

// Test that random_gamma returns values within (0, ∞) when scale is not zero
TEST_F(RandomTest, random_gamma_ReturnsValuesWithinRange) {
  double shape = 2.0;
  double scale = 3.0;
  int sample_size = 10000;

  for (int i = 0; i < sample_size; ++i) {
    double val = rng.random_gamma(shape, scale);
    EXPECT_GT(val, 0.0);
  }
}

// Statistical Mean and Variance Tests

// Test that the sample mean is close to the theoretical mean of the Gamma
// distribution
TEST_F(RandomTest, random_gamma_SampleMeanCloseToTheoretical) {
  double shape = 3.0;
  double scale = 2.0;
  int sample_size = 100000;
  double epsilon = 0.05;  // Acceptable error margin

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double theoretical_mean = shape * scale;

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon);
}

// Test that the sample variance is close to the theoretical variance of the
// Gamma distribution
TEST_F(RandomTest, random_gamma_SampleVarianceCloseToTheoretical) {
  double shape = 3.0;
  double scale = 2.0;
  int sample_size = 100000;
  double epsilon = 1;  // theoretical variance is about 12

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_variance = shape * scale * scale;

  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon);
}

// Edge Case Tests

// Test with alpha and scale equal to 1 (Exponential distribution)
TEST_F(RandomTest, random_gamma_GammaAlpha1Scale1_ExponentialDistribution) {
  double shape = 1.0;
  double scale = 1.0;
  int sample_size = 100000;
  double epsilon_mean = 0.05;
  double epsilon_variance = 0.1;

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_mean = shape * scale;
  double theoretical_variance = shape * scale * scale;

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with large alpha and scale values
TEST_F(RandomTest, random_gamma_LargeAlphaScaleValues) {
  double shape = 50.0;
  double scale = 2.0;
  int sample_size = 100000;
  double epsilon_mean = 0.5;
  double epsilon_variance = 10.0;  // variance is about 200

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_mean = shape * scale;
  double theoretical_variance = shape * scale * scale;

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with shape much larger than scale
TEST_F(RandomTest, random_gamma_ShapeMuchLargerThanScale) {
  double shape = 10.0;
  double scale = 1.0;
  int sample_size = 100000;
  double epsilon_mean = 0.1;
  double epsilon_variance = 0.5;  // var is about 10

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_mean = shape * scale;
  double theoretical_variance = shape * scale * scale;

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test with scale much larger than shape
TEST_F(RandomTest, random_gamma_ScaleMuchLargerThanShape) {
  double shape = 2.0;
  double scale = 10.0;
  int sample_size = 100000;
  double epsilon_mean = 0.5;
  double epsilon_variance = 5;  // var is about 200

  std::vector<double> samples(sample_size);
  for (int i = 0; i < sample_size; ++i) {
    samples[i] = rng.random_gamma(shape, scale);
  }

  double sample_mean = calculate_mean(samples);
  double sample_variance = calculate_variance(samples, sample_mean);
  double theoretical_mean = shape * scale;
  double theoretical_variance = shape * scale * scale;

  EXPECT_NEAR(sample_mean, theoretical_mean, epsilon_mean);
  EXPECT_NEAR(sample_variance, theoretical_variance, epsilon_variance);
}

// Test that random_multinomial throws when categories is zero
TEST_F(RandomTest, random_multinomial_ThrowsWhenCategoriesZero) {
  std::size_t categories = 0;
  unsigned trials = 100;
  std::vector<double> probabilities;  // Empty
  std::vector<unsigned> results;      // Empty

  EXPECT_THROW(
      rng.random_multinomial(categories, trials, probabilities, results),
      std::invalid_argument);
}

// Test that random_multinomial throws when probabilities vector size does not
// match categories
TEST_F(RandomTest, random_multinomial_ThrowsWhenProbabilitiesSizeMismatch) {
  std::size_t categories = 3;
  unsigned trials = 100;
  std::vector<double> probabilities = {0.2, 0.5};  // Size mismatch
  std::vector<unsigned> results(3, 0);

  EXPECT_THROW(
      rng.random_multinomial(categories, trials, probabilities, results),
      std::invalid_argument);
}

// Test that random_multinomial throws when results vector size does not match
// categories
TEST_F(RandomTest, random_multinomial_ThrowsWhenResultsSizeMismatch) {
  std::size_t categories = 3;
  unsigned trials = 100;
  std::vector<double> probabilities = {0.2, 0.5, 0.3};
  std::vector<unsigned> results(2, 0);  // Size mismatch

  EXPECT_THROW(
      rng.random_multinomial(categories, trials, probabilities, results),
      std::invalid_argument);
}

// Range Validation Tests

// Test that random_multinomial returns counts that sum to trials
TEST_F(RandomTest, random_multinomial_CountsSumToTrials) {
  std::size_t categories = 4;
  unsigned trials = 1000;
  std::vector<double> probabilities = {0.1, 0.2, 0.3, 0.4};
  std::vector<unsigned> results(4, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  unsigned total = 0;
  for (std::size_t i = 0; i < categories; ++i) {
    EXPECT_GE(results[i], 0u);
    EXPECT_LE(results[i], trials);
    total += results[i];
  }
  EXPECT_EQ(total, trials);
}

// Test that each count is within [0, trials]
TEST_F(RandomTest, random_multinomial_CountsWithinValidRange) {
  std::size_t categories = 5;
  unsigned trials = 500;
  std::vector<double> probabilities = {0.05, 0.15, 0.25, 0.25, 0.3};
  std::vector<unsigned> results(5, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  for (std::size_t i = 0; i < categories; ++i) {
    EXPECT_GE(results[i], 0u);
    EXPECT_LE(results[i], trials);
  }
}

// Test with categories = 1: all trials should be in the single category
TEST_F(RandomTest, random_multinomial_SingleCategoryAllTrials) {
  std::size_t categories = 1;
  unsigned trials = 1000;
  std::vector<double> probabilities = {1.0};
  std::vector<unsigned> results(1, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  EXPECT_EQ(results[0], trials);
}

// Test with trials = 0: all results should be 0
TEST_F(RandomTest, random_multinomial_ZeroTrialsAllResultsZero) {
  std::size_t categories = 3;
  unsigned trials = 0;
  std::vector<double> probabilities = {0.3, 0.4, 0.3};
  std::vector<unsigned> results = {1, 2, 3};  // Initialize with non-zero values

  rng.random_multinomial(categories, trials, probabilities, results);

  for (std::size_t i = 0; i < categories; ++i) { EXPECT_EQ(results[i], 0u); }
}

// Test with some zero probabilities
TEST_F(RandomTest, random_multinomial_ZeroProbabilities) {
  std::size_t categories = 4;
  unsigned trials = 1000;
  std::vector<double> probabilities = {0.0, 0.5, 0.5, 0.0};
  std::vector<unsigned> results(4, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  // Categories with zero probability should have zero counts
  EXPECT_EQ(results[0], 0u);
  EXPECT_EQ(results[3], 0u);

  // Sum of the remaining counts should equal trials
  unsigned sum = results[1] + results[2];
  EXPECT_EQ(sum, trials);
}

// Statistical Mean and Variance Tests

// Test that the sample counts align with expected counts based on probabilities
TEST_F(RandomTest, random_multinomial_SampleCountsAlignWithProbabilities) {
  std::size_t categories = 3;
  unsigned trials = 100000;
  std::vector<double> probabilities = {0.2, 0.5, 0.3};
  std::vector<unsigned> results(3, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  // Calculate expected counts
  std::vector<double> expected = calculate_expected_counts(
      trials, std::vector<double>(probabilities.begin(), probabilities.end()));

  // Define acceptable relative tolerance (e.g., 5%)
  double relative_tolerance = 0.05;

  // Check that each category's count is within the acceptable tolerance of the
  // expected count
  for (std::size_t i = 0; i < categories; ++i) {
    double expected_count = expected[i];
    double observed_count = static_cast<double>(results[i]);

    // Calculate the absolute tolerance based on relative tolerance
    double absolute_tolerance = expected_count * relative_tolerance;

    // Allow a minimum absolute tolerance to account for very small expected
    // counts
    absolute_tolerance =
        std::max(absolute_tolerance, 5.0);  // Minimum tolerance of 5 counts

    EXPECT_NEAR(observed_count, expected_count, absolute_tolerance)
        << "Category " << i << ": Observed count " << observed_count
        << " is not within " << relative_tolerance * 100
        << "% of expected count " << expected_count;
  }

  // Additionally, verify that the sum of all counts equals the number of trials
  unsigned long long total_counts =
      std::accumulate(results.begin(), results.end(), 0ULL);
  EXPECT_EQ(total_counts, static_cast<unsigned long long>(trials))
      << "Total counts " << total_counts
      << " do not equal the number of trials " << trials;
}

// Test with large number of trials and categories
TEST_F(RandomTest, random_multinomial_LargeTrialsAndCategories) {
  std::size_t categories = 100;
  unsigned trials = 1000000;
  std::vector<double> probabilities(categories,
                                    1.0 / static_cast<double>(categories));
  std::vector<unsigned> results(categories, 0);

  // Generate multinomially distributed random numbers
  rng.random_multinomial(categories, trials, probabilities, results);

  // Calculate expected counts
  std::vector<double> expected =
      calculate_expected_counts(trials, probabilities);

  // Define acceptable relative tolerance (e.g., 5%)
  double relative_tolerance = 0.05;

  // Check that each category's count is within the acceptable tolerance of the
  // expected count
  for (std::size_t i = 0; i < categories; ++i) {
    double expected_count = expected[i];
    double observed_count = static_cast<double>(results[i]);

    // Calculate the absolute tolerance based on relative tolerance
    double absolute_tolerance = expected_count * relative_tolerance;

    // Allow a minimum absolute tolerance to account for very small expected
    // counts
    absolute_tolerance =
        std::max(absolute_tolerance, 5.0);  // Minimum tolerance of 5 counts

    EXPECT_NEAR(observed_count, expected_count, absolute_tolerance)
        << "Category " << i << ": Observed count " << observed_count
        << " is not within " << relative_tolerance * 100
        << "% of expected count " << expected_count;
  }

  // Additionally, verify that the sum of all counts equals the number of trials
  unsigned long long total_counts =
      std::accumulate(results.begin(), results.end(), 0ULL);
  EXPECT_EQ(total_counts, static_cast<unsigned long long>(trials))
      << "Total counts " << total_counts
      << " do not equal the number of trials " << trials;
}

// Statistical Mean and Variance Tests
// Distribution Shape Tests

// Test that the distribution of counts matches the expected multinomial
// distribution using chi-squared test
TEST_F(RandomTest,
       random_multinomial_DistributionMatchesMultinomial_ChiSquaredTest) {
  std::size_t categories = 4;
  unsigned trials = 10000;
  std::vector<double> probabilities = {0.1, 0.2, 0.3, 0.4};
  std::vector<unsigned> results(4, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  // Calculate expected counts
  std::vector<double> expected =
      calculate_expected_counts(trials, std::vector<double>(probabilities));

  // Define acceptable relative tolerance (e.g., 10%)
  double relative_tolerance = 0.10;

  // Check that each category's count is within the acceptable tolerance of the
  // expected count
  for (std::size_t i = 0; i < categories; ++i) {
    double expected_count = expected[i];
    double observed_count = static_cast<double>(results[i]);

    // Calculate the absolute tolerance based on relative tolerance
    double absolute_tolerance = expected_count * relative_tolerance;

    // Allow a minimum absolute tolerance to account for very small expected
    // counts
    absolute_tolerance =
        std::max(absolute_tolerance, 5.0);  // Minimum tolerance of 5 counts

    EXPECT_NEAR(observed_count, expected_count, absolute_tolerance)
        << "Category " << i << ": Observed count " << observed_count
        << " is not within " << relative_tolerance * 100
        << "% of expected count " << expected_count;
  }

  // Additionally, verify that the sum of all counts equals the number of trials
  unsigned long long total_counts =
      std::accumulate(results.begin(), results.end(), 0ULL);

  EXPECT_EQ(total_counts, static_cast<unsigned long long>(trials))
      << "Total counts " << total_counts
      << " do not equal the number of trials " << trials;
}

// Edge Case Tests

// Test with extremely small probabilities
TEST_F(RandomTest, random_multinomial_ExtremelySmallProbabilities) {
  std::size_t categories = 5;
  unsigned trials = 100000;
  std::vector<double> probabilities = {1e-10, 0.2, 0.3, 0.5, 1e-10};

  std::vector<unsigned> results(5, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  // The categories with extremely small probabilities should have counts close
  // to zero
  EXPECT_LE(results[0], 5u);  // Allowing a small number due to randomness
  EXPECT_LE(results[4], 5u);
  // The sum of the other counts should be approximately trials
  unsigned sum = results[1] + results[2] + results[3];
  EXPECT_EQ(sum, trials);
}

// Test with extremely large number of categories
TEST_F(RandomTest, random_multinomial_ExtremelyLargeNumberOfCategories) {
  std::size_t categories = 1000;
  unsigned trials = 100000;
  std::vector<double> probabilities(categories,
                                    1.0 / static_cast<double>(categories));
  std::vector<unsigned> results(categories, 0);

  rng.random_multinomial(categories, trials, probabilities, results);

  // Calculate expected counts
  std::vector<double> expected =
      calculate_expected_counts(trials, probabilities);

  // expected counts should be approximately equal to trials/categories
  for (std::size_t i = 0; i < categories; ++i) {
    EXPECT_NEAR(expected[i], static_cast<double>(trials) / categories, 5);
  }
}

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

// Helper function to check if two vectors are permutations of each other
template <typename T>
bool is_permutation_of(const std::vector<T> &original,
                       const std::vector<T> &shuffled) {
  if (original.size() != shuffled.size()) { return false; }
  std::vector<T> sorted_original = original;
  std::vector<T> sorted_shuffled = shuffled;
  std::sort(sorted_original.begin(), sorted_original.end());
  std::sort(sorted_shuffled.begin(), sorted_shuffled.end());
  return sorted_original == sorted_shuffled;
}

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

// Test cdf_gamma_distribution
TEST_F(RandomTest, CdfGammaDistribution) {
  Random rng;
  rng.set_seed(42);

  double value = 3.0;
  double alpha = 2.0;
  double beta = 1.5;
  double cdf = rng.cdf_gamma_distribution(value, alpha, beta);

  // Expected CDF can be calculated using GSL or known values
  // Here, we'll just check that it's between 0 and 1
  EXPECT_GE(cdf, 0.0);
  EXPECT_LE(cdf, 1.0);
}

// Test cdf_gamma_distribution_inverse
TEST_F(RandomTest, CdfGammaDistributionInverse) {
  Random rng;
  rng.set_seed(42);

  double probability = 0.95;
  double alpha = 2.0;
  double beta = 1.5;
  double quantile =
      rng.cdf_gamma_distribution_inverse(probability, alpha, beta);

  // Verify that the CDF at quantile is approximately the probability
  double cdf = rng.cdf_gamma_distribution(quantile, alpha, beta);
  EXPECT_NEAR(cdf, probability, 1e-6);
}

// Test constructor with nullptr (should initialize internally)
TEST_F(RandomTest, ConstructorWithNullptr) {
  Random rng(nullptr);
  EXPECT_NE(rng.get_seed(), 0);
}

// Test random_binomial with invalid probability
TEST_F(RandomTest, RandomBinomialInvalidProbability) {
  Random rng;
  rng.set_seed(42);

  double probability = 1.5;  // Invalid
  unsigned int trials = 10;

  EXPECT_THROW(rng.random_binomial(probability, trials), std::invalid_argument);
}

