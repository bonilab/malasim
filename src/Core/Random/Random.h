// Random.h

#ifndef RANDOM_H
#define RANDOM_H

#include <gsl/gsl_rng.h>

#include <cstddef>
#include <cstdint>
#include <memory>

/**
 * @class Random
 * @brief Encapsulates random number generation functionalities using GSL.
 *
 * The Random class provides various methods to generate random numbers
 * following different distributions. It manages a GSL random number generator
 * (RNG) instance, ensuring proper resource management and offering a clean
 * interface for simulations.
 */
class Random {
public:
  // Delete copy constructor and copy assignment operator
  Random(const Random &) = delete;
  Random &operator=(const Random &) = delete;

  // Delete move constructor and move assignment operator
  Random(Random &&) = delete;
  Random &operator=(Random &&) = delete;

  /**
   * @brief Constructs a Random number generator.
   *
   * If an external GSL RNG is provided, the class takes ownership of it.
   * Otherwise, it initializes a new RNG with a default or specified seed.
   *
   * @param external_rng Pointer to an external GSL RNG. Defaults to nullptr.
   */
  explicit Random(gsl_rng* external_rng = nullptr);

  /**
   * @brief Destructor.
   *
   * Ensures proper resource deallocation of the GSL RNG.
   */
  ~Random();

  /**
   * @brief Retrieves the current RNG seed.
   *
   * @return uint64_t The seed value.
   */
  [[nodiscard]] uint64_t get_seed() const noexcept;

  /**
   * @brief Sets a new seed for the RNG.
   *
   * @param new_seed The seed value to set.
   */
  void set_seed(uint64_t new_seed);

  // Random number generation methods

  /**
   * @brief Generates a Poisson-distributed random number.
   *
   * @param mean The mean of the Poisson distribution.
   * @return int A random number following the Poisson distribution.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  int random_poisson(double mean);

  /**
   * @brief Generates a uniform random integer in [0, upper_bound).
   *
   * @param upper_bound The exclusive upper bound for the random number.
   * @return uint64_t A uniformly distributed random integer.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  uint64_t random_uniform(uint64_t upper_bound);

  /**
   * @brief Generates a uniform random integer in [lower_bound, upper_bound).
   *
   * @param lower_bound The inclusive lower bound.
   * @param upper_bound The exclusive upper bound.
   * @return uint64_t A uniformly distributed random integer.
   *
   * @throws std::invalid_argument If lower_bound is not less than upper_bound.
   * @throws std::runtime_error If RNG is not initialized.
   */
  uint64_t random_uniform(uint64_t lower_bound, uint64_t upper_bound);

  /**
   * @brief Generates a uniform random double in [from, to).
   *
   * @param from The inclusive lower bound.
   * @param to The exclusive upper bound.
   * @return double A uniformly distributed random double.
   *
   * @throws std::invalid_argument If from is not less than to.
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_uniform(double from, double to);

  /**
   * @brief Generates a uniform random double in [0, 1).
   *
   * @return double A uniformly distributed random double.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_uniform();

  /**
   * @brief Generates a normally distributed random double with given mean and
   * standard deviation.
   *
   * @param mean The mean of the normal distribution.
   * @param standard_deviation The standard deviation of the normal
   * distribution.
   * @return double A random number following the normal distribution.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_normal(double mean, double standard_deviation);

  /**
   * @brief Generates a truncated normally distributed random double within ±3
   * standard deviations.
   *
   * @param mean The mean of the normal distribution.
   * @param standard_deviation The standard deviation of the normal
   * distribution.
   * @return double A truncated normally distributed random double.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_normal_truncated(double mean, double standard_deviation);

  /**
   * @brief Generates a normally distributed random integer with given mean and
   * standard deviation.
   *
   * @param mean The mean of the normal distribution.
   * @param standard_deviation The standard deviation of the normal
   * distribution.
   * @return int A random integer following the normal distribution.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  int random_normal(int mean, int standard_deviation);

  /**
   * @brief Generates a truncated normally distributed random integer within ±3
   * standard deviations.
   *
   * @param mean The mean of the normal distribution.
   * @param standard_deviation The standard deviation of the normal
   * distribution.
   * @return int A truncated normally distributed random integer.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  int random_normal_truncated(int mean, int standard_deviation);

  /**
   * @brief Generates a Beta-distributed random double.
   *
   * @param alpha The alpha parameter of the Beta distribution.
   * @param beta The beta parameter of the Beta distribution.
   * @return double A random number following the Beta distribution.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_beta(double alpha, double beta);

  /**
   * @brief Generates a Gamma-distributed random double.
   *
   * @param shape The shape parameter of the Gamma distribution.
   * @param scale The scale parameter of the Gamma distribution.
   * @return double A random number following the Gamma distribution.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double random_gamma(double shape, double scale);

  /**
   * @brief Computes the CDF of the Gamma distribution.
   *
   * @param value The value at which to evaluate the CDF.
   * @param shape The shape parameter of the Gamma distribution.
   * @param scale The scale parameter of the Gamma distribution.
   * @return double The CDF value.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double cdf_gamma_distribution(double value, double alpha, double beta);

  /**
   * @brief Computes the inverse CDF (quantile function) of the Gamma
   * distribution.
   *
   * @param probability The probability value for which to compute the quantile.
   * @param shape The shape parameter of the Gamma distribution.
   * @param scale The scale parameter of the Gamma distribution.
   * @return double The quantile corresponding to the given probability.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double cdf_gamma_distribution_inverse(double probability, double alpha,
                                        double beta);

  /**
   * @brief Generates a flat-distributed random double in [from, to).
   *
   * This is identical to `random_uniform_double`.
   *
   * @param from The inclusive lower bound.
   * @param to The exclusive upper bound.
   * @return double A flat-distributed random double.
   */
  double random_flat(double from, double to);

  /**
   * @brief Generates multinomial random variables.
   *
   * @param categories Number of categories.
   * @param trials Number of trials.
   * @param probabilities Pointer to array of probabilities (size `categories`).
   * @param results Pointer to array where results will be stored (size
   * `categories`).
   *
   * @throws std::invalid_argument If `categories` is zero.
   * @throws std::runtime_error If RNG is not initialized.
   */
  void random_multinomial(std::size_t categories, unsigned trials,
                          const double* probabilities, unsigned* results);

  /**
   * @brief Computes the CDF of the standard normal distribution.
   *
   * @param value The value at which to evaluate the CDF.
   * @return double The CDF value.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  double cdf_standard_normal_distribution(double value);

  /**
   * @brief Generates a binomially distributed random integer.
   *
   * @param probability The probability of success in each trial.
   * @param trials The number of trials.
   * @return int A binomially distributed random integer.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  unsigned int random_binomial(double probability, unsigned int trials);

  /**
   * @brief Shuffles elements in an array using the current random generator.
   *
   * @param array Pointer to the first element of the array.
   * @param element_count Number of elements in the array.
   * @param element_size Size of each element in bytes.
   *
   * @throws std::runtime_error If RNG is not initialized.
   */
  void shuffle(void* array, std::size_t element_count,
               std::size_t element_size) const;

private:
  uint64_t seed_;
  static constexpr double TRUNCATION_LIMIT = 3.0;

  // Custom deleter for gsl_rng
  struct GslRngDeleter {
    void operator()(gsl_rng* rng) const {
      if (rng != nullptr) { gsl_rng_free(rng); }
    }
  };

  // Unique pointer managing gsl_rng with custom deleter
  std::unique_ptr<gsl_rng, GslRngDeleter> rng_;

  /**
   * @brief Initializes the GSL random number generator with a given seed.
   *
   * If the provided seed is zero, it uses `std::random_device` to generate a
   * random seed.
   *
   * @param initial_seed Seed value. Defaults to 0.
   *
   * @throws std::runtime_error If RNG allocation fails.
   */
  void initialize(uint64_t initial_seed = 0);
};

#endif  // RANDOM_H

