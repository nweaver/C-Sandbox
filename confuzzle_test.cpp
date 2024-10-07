#include <gtest/gtest.h>

extern "C" {
    #include "confuzzle.h"
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest, KnownFailure) {
    EXPECT_TRUE(false) << "False is not true!";
}

TEST(TestC, ConfuzzleTest) {
    EXPECT_EQ(confuzzle((char *)"abc"), 6) << "Just a test";
}