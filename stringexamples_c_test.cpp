#include <gtest/gtest.h>
#include <string>
extern "C" {
  #include "stringexamples_c.h"
}

// Demonstrate some basic assertions.
TEST(CStringTest, BasicAssertions) {
  // This should be equal by the std::string equality, which is
  // what we REALLY want.
  char *ret;
  ret = toupper_str("ThiS iS a TeST");
  EXPECT_STREQ("THIS IS A TEST", ret);
  EXPECT_STRNE("This IS A TEST", ret);
  free(ret);
}