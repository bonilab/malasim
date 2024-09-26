// helpers/test_helpers.cpp

#include "test_helpers.h"

// Calculate sample mean for unsigned int data
double calculate_mean(const std::vector<unsigned int> &data) {
  if (data.empty()) {
    throw std::invalid_argument("Cannot calculate mean of empty data.");
  }
  unsigned long long sum = std::accumulate(data.begin(), data.end(), 0ULL);
  return static_cast<double>(sum) / static_cast<double>(data.size());
}

// Calculate sample variance for unsigned int data
double calculate_variance(const std::vector<unsigned int> &data, double mean) {
  if (data.empty()) {
    throw std::invalid_argument("Cannot calculate variance of empty data.");
  }
  double sum_sq_diff = 0.0;
  for (const auto &val : data) {
    double diff = static_cast<double>(val) - mean;
    sum_sq_diff += diff * diff;
  }
  return sum_sq_diff / static_cast<double>(data.size());
}

// Calculate sample mean for double data
double calculate_mean(const std::vector<double> &data) {
  if (data.empty()) {
    throw std::invalid_argument("Cannot calculate mean of empty data.");
  }
  double sum = std::accumulate(data.begin(), data.end(), 0.0);
  return sum / static_cast<double>(data.size());
}

// Calculate sample standard deviation for double data
double calculate_stddev(const std::vector<double> &data, double mean) {
  if (data.empty()) {
    throw std::invalid_argument(
        "Cannot calculate standard deviation of empty data.");
  }
  double sum_sq = 0.0;
  for (const auto &val : data) { sum_sq += (val - mean) * (val - mean); }
  return std::sqrt(sum_sq / static_cast<double>(data.size()));
}

// Calculate sample variance for double data
double calculate_variance(const std::vector<double> &data, double mean) {
  if (data.empty()) {
    throw std::invalid_argument("Cannot calculate variance of empty data.");
  }
  double sum_sq = 0.0;
  for (const auto &val : data) { sum_sq += (val - mean) * (val - mean); }
  return sum_sq / static_cast<double>(data.size());
}

// Calculate expected counts based on probabilities
std::vector<double> calculate_expected_counts(
    unsigned trials, const std::vector<double> &probabilities) {
  std::vector<double> expected;
  for (const auto &p : probabilities) { expected.push_back(trials * p); }
  return expected;
}

// Calculate chi-squared statistic
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

