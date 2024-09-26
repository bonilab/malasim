// helpers/test_helpers.h

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

// Helper function declarations
double calculate_mean(const std::vector<unsigned int> &data);
double calculate_variance(const std::vector<unsigned int> &data, double mean);
double calculate_mean(const std::vector<double> &data);
double calculate_stddev(const std::vector<double> &data, double mean);
double calculate_variance(const std::vector<double> &data, double mean);
std::vector<double> calculate_expected_counts(
    unsigned trials, const std::vector<double> &probabilities);
double calculate_chi_squared(const std::vector<unsigned> &observed,
                             const std::vector<double> &expected);

// Template function to check if two vectors are permutations of each other
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

#endif  // TEST_HELPERS_H

