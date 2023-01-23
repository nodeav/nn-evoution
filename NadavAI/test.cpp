#include <gtest/gtest.h>
#include "utils.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

//  EXPECT_EQ(1, 0);
}

TEST(GetDistanceTest, sanity) {
  EXPECT_EQ(getDistance({0, 1}, {0, 2}), 1.0);
  EXPECT_EQ(getDistance({0, 0}, {3, 4}), 5.0);
}

TEST(GetAngleTest, sanity) {
    EXPECT_EQ(getAngle({0, 1}, {0, 2}), Radian(M_PI));
}

TEST(RadianTest, between) {
    Radian rad(0.5);
    EXPECT_TRUE(rad.between(0, 1));
    EXPECT_TRUE(rad.between(0.5, 0.5));
    EXPECT_TRUE(rad.between(0., 0.5));
    EXPECT_TRUE(rad.between(2*M_PI, 0.5));
    EXPECT_TRUE(rad.between(2*M_PI-0.01, 0.5));
    EXPECT_FALSE(rad.between(0., 0.49));
    EXPECT_FALSE(rad.between(2*M_PI-0.01, 0.49));
    EXPECT_FALSE(rad.between(0.51, 0.49));
}