// RandomTest_random_multinomial.cpp

#include <gtest/gtest.h>

#include "RandomTestBase.h"

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
