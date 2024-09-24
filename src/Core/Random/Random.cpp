/*
 * Random.cpp
 *
 * This file implements the random number class for use by the simulation.
 */
#include "Random.h"

#include <fmt/format.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>

#include <cmath>
#include <cstdint>
#include <random>

// Constructor
Random::Random(gsl_rng* rng, uint64_t seed) : seed_(seed) {
  if (rng != nullptr) {
    // If an external RNG is provided, take ownership using the unique_ptr
    rng_.reset(rng);
  } else {
    // Initialize with default seed
    initialize();
  }
}

// Destructor
Random::~Random() = default;

void Random::initialize(uint64_t initial_seed) {
  // Select the Mersenne Twister 19937 generator
  const gsl_rng_type* rng_type = gsl_rng_mt19937;

  // Allocate the GSL RNG and assign it to the unique_ptr
  rng_.reset(gsl_rng_alloc(rng_type));
  if (!rng_) {
    throw std::runtime_error("Failed to allocate GSL random number generator.");
  }

  // Use std::random_device to generate a random seed if seed is 0
  std::random_device rd;
  seed_ = (initial_seed == 0) ? rd() : initial_seed;

  // Log the seed value
  // LOG(INFO) << fmt::format("Random initializing with seed: {}", seed_);

  // Set the seed for the GSL RNG
  gsl_rng_set(rng_.get(), seed_);
}

// Getter for seed
uint64_t Random::get_seed() const noexcept { return seed_; }

// Setter for seed
void Random::set_seed(uint64_t new_seed) {
  if (rng_) { gsl_rng_set(rng_.get(), new_seed); }
  seed_ = new_seed;
}

// Generates a Poisson-distributed random number
int Random::random_poisson(double poissonMean) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return static_cast<int>(gsl_ran_poisson(rng_.get(), poissonMean));
}

// Generates a uniform random integer in [0, range)
uint64_t Random::random_uniform(uint64_t upper_bound) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_rng_uniform_int(rng_.get(), upper_bound);
}

// Generates a uniform random integer in [from, to)
uint64_t Random::random_uniform(uint64_t lower_bound, uint64_t upper_bound) {
  if (lower_bound >= upper_bound) {
    throw std::invalid_argument("Parameter 'from' must be less than 'to'.");
  }
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return lower_bound
         + gsl_rng_uniform_int(rng_.get(), upper_bound - lower_bound);
}

// Generates a uniform random double in [from, to)
double Random::random_uniform(double from, double to) {
  if (from >= to) {
    throw std::invalid_argument("Parameter 'from' must be less than 'to'.");
  }
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_ran_flat(rng_.get(), from, to);
}

// Generates a uniform random double in [0, 1)
double Random::random_uniform() {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_rng_uniform(rng_.get());
}

// Generates a normally distributed random double with given mean and standard
// deviation
double Random::random_normal(double mean, double standard_deviation) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return mean + gsl_ran_gaussian(rng_.get(), standard_deviation);
}

// Generates a normally distributed random integer with given mean and standard
// deviation
int Random::random_normal(int mean, int standard_deviation) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return static_cast<int>(
      mean
      + std::round(gsl_ran_gaussian(rng_.get(),
                                    static_cast<double>(standard_deviation))));
}

// Generates a truncated normally distributed random double within ±3 standard
// deviations
double Random::random_normal_truncated(double mean, double standard_deviation) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  double value = gsl_ran_gaussian(rng_.get(), standard_deviation);
  while (std::abs(value) > TRUNCATION_LIMIT * standard_deviation) {
    value = gsl_ran_gaussian(rng_.get(), standard_deviation);
  }
  return mean + value;
}

// Generates a truncated normally distributed random integer within ±3 standard
// deviations
int Random::random_normal_truncated(int mean, int standard_deviation) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  double value =
      gsl_ran_gaussian(rng_.get(), static_cast<double>(standard_deviation));
  while (std::abs(value) > TRUNCATION_LIMIT * standard_deviation) {
    // Regenerate the value until it falls within the truncation limit
    value =
        gsl_ran_gaussian(rng_.get(), static_cast<double>(standard_deviation));
  }
  return static_cast<int>(mean + std::round(value));
}

// Generates a Beta-distributed random double
double Random::random_beta(double alpha, double beta) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (beta == 0.0) { return alpha; }
  return gsl_ran_beta(rng_.get(), alpha, beta);
}

// Generates a Gamma-distributed random double
double Random::random_gamma(double shape, double scale) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (scale == 0) { return shape; }
  return gsl_ran_gamma(rng_.get(), shape, scale);
}

// Computes the CDF of the Gamma distribution
double Random::cdf_gamma_distribution(double value, double alpha, double beta) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (alpha <= 0.0 || beta < 0) {
    throw std::invalid_argument(
        "Parameters 'alpha' and 'beta' must be greater than 0.");
  }
  if (beta == 0) { return 1.0; }
  return gsl_cdf_gamma_P(value, alpha, beta);
}

// Computes the inverse CDF (quantile function) of the Gamma distribution
double Random::cdf_gamma_distribution_inverse(double probability, double alpha,
                                              double beta) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_cdf_gamma_Pinv(probability, alpha, beta);
}

// Generates a flat-distributed random double in [from, to)
double Random::random_flat(double from, double to) {
  // This is identical to randomUniformDouble
  return random_uniform(from, to);
}

// Generates multinomially distributed random numbers

void Random::random_multinomial(std::size_t categories, unsigned trials,
                                const double* probabilities,
                                unsigned* results) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (categories == 0) {
    throw std::invalid_argument(
        "Parameter 'categories' must be greater than 0.");
  }
  if (probabilities == nullptr || results == nullptr) {
    throw std::invalid_argument(
        "Parameters 'probabilities' and 'results' must not be null.");
  }
  gsl_ran_multinomial(rng_.get(), static_cast<unsigned int>(categories), trials,
                      probabilities, results);
}

// Computes the CDF of the standard normal distribution
double Random::cdf_standard_normal_distribution(double value) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_cdf_ugaussian_P(value);
}

// Generates a binomially distributed random integer
unsigned int Random::random_binomial(double probability, unsigned int trials) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (probability < 0.0 || probability > 1.0) {
    throw std::invalid_argument("Parameter 'probability' must be in [0, 1].");
  }
  return gsl_ran_binomial(rng_.get(), probability, trials);
}

// Shuffles elements in a range using the current random generator
void Random::shuffle(void* array, std::size_t element_count,
                     std::size_t element_size) const {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (array == nullptr) {
    throw std::invalid_argument("Parameter 'array' must not be null.");
  }
  gsl_ran_shuffle(rng_.get(), array, element_count, element_size);
}