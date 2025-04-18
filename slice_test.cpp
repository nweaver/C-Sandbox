#include <gtest/gtest.h>
#include <string>
#include "slice.hpp"
#include <iostream>

TEST(SliceTest, AppendingOnEnd)
{
    Slice<int> foo;
    std::cout << "Running \n";
    for(auto x = 0; x < 10; ++x){
        foo.push_back(x);
        ASSERT_EQ(foo[x], x);
    }
    for(auto x = 0; x < 10; ++x){
        ASSERT_EQ(foo[x], x);
    }
    Slice<int> bar(foo, 1, 4);
    for(auto x = 0; x < 4; ++x){
        ASSERT_EQ(bar[x], x+1);
    }
    for(auto x = 4; x < 10; ++x){
        EXPECT_THROW(bar[x], SliceOutOfBoundsException);
    }
    EXPECT_THROW(Slice<int> baz(bar, -10, 10), SliceException);
    bar.push_back(32);
    EXPECT_EQ(foo[5], 32);
    std::vector<int> baz({1, 2, 3, 4});
    Slice<int> garplay(baz, 1, 2);
    EXPECT_EQ(garplay[0], 2);
    EXPECT_EQ(garplay[1], 3);
    EXPECT_THROW(garplay[3], SliceOutOfBoundsException);
}
