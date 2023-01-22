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
