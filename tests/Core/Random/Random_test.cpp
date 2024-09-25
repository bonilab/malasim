// Random_test.cpp

#include "Core/Random/Random.h"

#include <gsl/gsl_rng.h>
#include <gtest/gtest.h>

#include <cmath>

// Test fixture for Random class
class RandomTest : public ::testing::Test {
protected:
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
  Random rng;
  uint64_t initial_seed = rng.get_seed();
  EXPECT_NE(initial_seed, 0);

  rng.set_seed(67890);
  EXPECT_EQ(rng.get_seed(), 67890);

  // Reset seed and check
  rng.set_seed(123456789);
  EXPECT_EQ(rng.get_seed(), 123456789);
}

// Test random_poisson
TEST_F(RandomTest, RandomPoisson) {
  // Initialize RNG with a fixed seed
  Random rng;
  rng.set_seed(42);  // Fixed seed for reproducibility

  double lambda = 3.0;
  int poisson1 = rng.random_poisson(lambda);
  int poisson2 = rng.random_poisson(lambda);

  // Expected values may vary based on GSL's implementation
  // Here, we'll just check that the values are within a reasonable range
  EXPECT_GE(poisson1, 0);
  EXPECT_GE(poisson2, 0);
}

// Test random_uniform with upper_bound
TEST_F(RandomTest, RandomUniformUpperBound) {
  Random rng;
  rng.set_seed(42);

  uint64_t upper = 100;
  uint64_t uniform1 = rng.random_uniform(upper);
  uint64_t uniform2 = rng.random_uniform(upper);

  EXPECT_LT(uniform1, upper);
  EXPECT_LT(uniform2, upper);
}

// Test random_uniform with lower_bound and upper_bound
TEST_F(RandomTest, RandomUniformRange) {
  Random rng;
  rng.set_seed(42);

  uint64_t lower = 50;
  uint64_t upper = 150;
  uint64_t uniform1 = rng.random_uniform(lower, upper);
  uint64_t uniform2 = rng.random_uniform(lower, upper);

  EXPECT_GE(uniform1, lower);
  EXPECT_LT(uniform1, upper);
  EXPECT_GE(uniform2, lower);
  EXPECT_LT(uniform2, upper);
}

// Test random_uniform with double range
TEST_F(RandomTest, RandomUniformDoubleRange) {
  Random rng;
  rng.set_seed(42);

  double from = 5.0;
  double to = 10.0;
  double uniform1 = rng.random_uniform(from, to);
  double uniform2 = rng.random_uniform(from, to);

  EXPECT_GE(uniform1, from);
  EXPECT_LT(uniform1, to);
  EXPECT_GE(uniform2, from);
  EXPECT_LT(uniform2, to);
}

// Test random_uniform without bounds
TEST_F(RandomTest, RandomUniformNoBounds) {
  Random rng;
  rng.set_seed(42);

  double uniform1 = rng.random_uniform();
  double uniform2 = rng.random_uniform();

  EXPECT_GE(uniform1, 0.0);
  EXPECT_LT(uniform1, 1.0);
  EXPECT_GE(uniform2, 0.0);
  EXPECT_LT(uniform2, 1.0);
}

// Test random_normal with double parameters
TEST_F(RandomTest, RandomNormalDouble) {
  Random rng;
  rng.set_seed(42);

  double mean = 0.0;
  double std_dev = 1.0;
  double normal1 = rng.random_normal(mean, std_dev);
  double normal2 = rng.random_normal(mean, std_dev);

  // Since the seed is fixed, we can expect specific values
  // However, GSL's random number generator outputs can be checked against
  // expected Alternatively, ensure the values are within expected statistical
  // ranges Here, we'll check if the values are finite
  EXPECT_TRUE(std::isfinite(normal1));
  EXPECT_TRUE(std::isfinite(normal2));
}

// Test random_normal with integer parameters
TEST_F(RandomTest, RandomNormalInt) {
  Random rng;
  rng.set_seed(42);

  int mean = 5;
  int std_dev = 2;
  int normal1 = rng.random_normal(mean, std_dev);
  int normal2 = rng.random_normal(mean, std_dev);

  // Check that the values are integers
  // Since the method returns int, this is implicit
  // Alternatively, check if they are within a reasonable range
  EXPECT_GE(normal1, mean - (3 * std_dev));
  EXPECT_LE(normal1, mean + (3 * std_dev));
  EXPECT_GE(normal2, mean - (3 * std_dev));
  EXPECT_LE(normal2, mean + (3 * std_dev));
}

// Test random_beta
TEST_F(RandomTest, RandomBeta) {
  Random rng;
  rng.set_seed(42);

  double alpha = 2.0;
  double beta = 5.0;
  double beta1 = rng.random_beta(alpha, beta);
  double beta2 = rng.random_beta(alpha, beta);

  EXPECT_GE(beta1, 0.0);
  EXPECT_LE(beta1, 1.0);
  EXPECT_GE(beta2, 0.0);
  EXPECT_LE(beta2, 1.0);
}

// Test random_gamma
TEST_F(RandomTest, RandomGamma) {
  Random rng;
  rng.set_seed(42);

  double shape = 2.0;
  double scale = 1.5;
  double gamma1 = rng.random_gamma(shape, scale);
  double gamma2 = rng.random_gamma(shape, scale);

  EXPECT_GT(gamma1, 0.0);
  EXPECT_GT(gamma2, 0.0);
}

// Test random_binomial
TEST_F(RandomTest, RandomBinomial) {
  Random rng;
  rng.set_seed(42);

  double probability = 0.5;
  unsigned int trials = 10;
  unsigned int binomial1 = rng.random_binomial(probability, trials);
  unsigned int binomial2 = rng.random_binomial(probability, trials);

  EXPECT_LE(binomial1, trials);
  EXPECT_LE(binomial2, trials);
}

// Test shuffle
TEST_F(RandomTest, Shuffle) {
  Random rng;
  rng.set_seed(42);
  // Define the arrays using std::array
  std::array<int, 5> array = {1, 2, 3, 4, 5};
  std::array<int, 5> original = {1, 2, 3, 4, 5};

  rng.shuffle(array.data(), array.size(), sizeof(int));

  // Check that the array is shuffled (i.e., not in the original order)
  bool is_shuffled = false;
  for (int i = 0; i < array.size(); ++i) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    if (array[i] != original[i]) {
      is_shuffled = true;
      break;
    }
  }
  EXPECT_TRUE(is_shuffled);

  // Optionally, verify against expected shuffled result with fixed seed
  // This requires knowing the exact shuffle outcome
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

// Test invalid arguments for random_uniform (lower_bound >= upper_bound)
TEST_F(RandomTest, RandomUniformInvalidRange) {
  Random rng;
  rng.set_seed(42);

  uint64_t lower = 100;
  uint64_t upper = 50;

  EXPECT_THROW(rng.random_uniform(lower, upper), std::invalid_argument);
}

// Test invalid arguments for random_uniform with doubles
TEST_F(RandomTest, RandomUniformDoubleInvalidRange) {
  Random rng;
  rng.set_seed(42);

  double from = 10.0;
  double to = 5.0;

  EXPECT_THROW(rng.random_uniform(from, to), std::invalid_argument);
}

// Test random_multinomial with categories = 0
TEST_F(RandomTest, RandomMultinomialZeroCategories) {
  Random rng;
  rng.set_seed(42);

  std::size_t categories = 0;
  unsigned trials = 10;
  std::array<double, 1> probabilities = {1.0};
  std::array<unsigned, 1> results = {0};

  EXPECT_THROW(rng.random_multinomial(categories, trials, probabilities.data(),
                                      results.data()),
               std::invalid_argument);
}

// Test random_binomial with invalid probability
TEST_F(RandomTest, RandomBinomialInvalidProbability) {
  Random rng;
  rng.set_seed(42);

  double probability = 1.5;  // Invalid
  unsigned int trials = 10;

  EXPECT_THROW(rng.random_binomial(probability, trials), std::invalid_argument);
}

