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
#include <cstddef>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <vector>

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

// Generates a uniform random double in [0, 1)
double Random::random_uniform() {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_rng_uniform(rng_.get());
}

// Generates a Beta-distributed random double
double Random::random_beta(double alpha, double beta) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (alpha <= 0.0 || beta < 0) {
    throw std::invalid_argument(
        "Parameters 'alpha' and 'beta' must be greater than 0.");
  }
  if (beta == 0.0) { return alpha; }
  return gsl_ran_beta(rng_.get(), alpha, beta);
}

// Generates a Gamma-distributed random double
double Random::random_gamma(double shape, double scale) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (shape <= 0.0 || scale < 0) {
    throw std::invalid_argument(
        "Parameters 'shape' and 'scale' must be greater than 0.");
  }
  if (scale == 0) { return shape; }
  return gsl_ran_gamma(rng_.get(), shape, scale);
}

// Generates multinomially distributed random numbers
void Random::random_multinomial(std::size_t categories, unsigned trials,
                                const std::vector<double> &probabilities,
                                std::vector<unsigned> &results) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  if (categories == 0) {
    throw std::invalid_argument(
        "Parameter 'categories' must be greater than 0.");
  }
  if (probabilities.size() != categories || results.size() != categories) {
    throw std::invalid_argument(
        "Size of 'probabilities' and 'results' must match 'categories'.");
  }
  gsl_ran_multinomial(rng_.get(), static_cast<unsigned int>(categories), trials,
                      probabilities.data(), results.data());
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
  if (probability < 0.0 || probability > 1.0) {
    throw std::invalid_argument("Parameter 'probability' must be in [0, 1].");
  }
  return gsl_cdf_gamma_Pinv(probability, alpha, beta);
}

// Computes the CDF of the standard normal distribution
double Random::cdf_standard_normal_distribution(double value) {
  if (!rng_) {
    throw std::runtime_error("Random number generator not initialized.");
  }
  return gsl_cdf_ugaussian_P(value);
}

