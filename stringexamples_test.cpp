#include <gtest/gtest.h>
#include <string>
#include "stringexamples.hpp"

// Demonstrate some basic assertions.
TEST(StringTest, BasicAssertions) {
  // This should be equal by the std::string equality, which is
  // what we REALLY want.
  EXPECT_EQ("THIS IS A TEST", toupper("ThiS iS a TeST"));
  EXPECT_NE("This IS A TEST", toupper("ThiS iS a TeST"));


  EXPECT_EQ("gnip", reverse("ping"));
  EXPECT_NE("gnip ", reverse("ping"));
  EXPECT_EQ("test", stripl(" \t\n\rtest"));
  EXPECT_EQ("", stripl(" \t\n   "));
  EXPECT_EQ("test", stripl("test"));

  EXPECT_EQ("test  ", stripl(" \t\ntest  "));
  EXPECT_EQ(" \t\ntest", stripr(" \t\ntest \t"));

  // But this also shows the limitation of C++'s string handling,
  // by passing by value we end up doing extra copying.  But such
  // is life if we actually want sane strings
}