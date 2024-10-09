#include <gtest/gtest.h>
#include <string>
#include "llist.hpp"

// Demonstrate some basic assertions.
TEST(LinkedListTest, AppendingOnEnd) {
    LinkedList<int> foo, bar;
    EXPECT_EQ(foo.len(), 0);
    for(auto i = 0; i < 10; ++i){
        EXPECT_THROW(foo[i], LinkedListException);
        foo.append(i);
        bar.prepend(i);
        EXPECT_EQ(foo[i], i);
        foo[i] = 2*i;
        EXPECT_EQ(foo[i], 2 * i);
    }
    for(auto i = 0; i < 10; ++i){
        EXPECT_EQ(bar[i], 9-i);
    }
    auto j = 0;
    for(auto i : foo){
        EXPECT_EQ(i, j * 2);
        i = j;
    }
    j = 0;
    for(auto i : foo){
        EXPECT_EQ(i, j);
    }
}