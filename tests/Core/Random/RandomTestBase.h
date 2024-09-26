// RandomTestBase.h

#ifndef RANDOM_TEST_BASE_H
#define RANDOM_TEST_BASE_H

#include <gtest/gtest.h>

#include "Core/Random/Random.h"
#include "helpers/test_helpers.h"

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

#endif  // RANDOM_TEST_BASE_H

