// RandomTest_random_normal.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"
#include "helpers/test_helpers.h"

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
