#include "example.h"

#include <gtest/gtest.h>

TEST(AdditionTest, PositiveNumbers) { EXPECT_EQ(add(1, 2), 3); }

TEST(AdditionTest, NegativeNumbers) { EXPECT_EQ(add(-1, -2), -3); }
