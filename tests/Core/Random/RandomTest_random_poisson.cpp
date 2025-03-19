// RandomTest_random_poisson.cpp

#include <gtest/gtest.h>
#include "RandomTestBase.h"

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

