#include <gtest/gtest.h>
#include <string>
#include "llist.hpp"

// Demonstrate some basic assertions.
TEST(LinkedListTest, AppendingOnEnd)
{
    LinkedList<int> foo, bar;
    EXPECT_EQ(foo.len(), 0);
    for (auto i = 0; i < 10; ++i)
    {
        EXPECT_EQ(foo.len(), i);
        EXPECT_EQ(bar.len(), i);
        EXPECT_THROW(foo[i], LinkedListException);
        foo.append(i);
        bar.prepend(i);
        EXPECT_EQ(foo[i], i);
        foo[i] = 2 * i;
        EXPECT_EQ(foo[i], 2 * i);
    }
    for (auto i = 0; i < 10; ++i)
    {
        EXPECT_EQ(bar[i], 9 - i);
        EXPECT_EQ(foo[i], 2 * i);
    }
    auto j = 0;
    for (auto &i : foo)
    {
        EXPECT_EQ(i, j * 2);
        i = j;
        j++;
    }
    j = 0;
    for (auto i : foo)
    {
        EXPECT_EQ(i, j);
        j++;
    }
    EXPECT_EQ(to_string(foo), "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");

    LinkedList<std::string> baz;
    baz.append("Hola");
    baz.append("Hello");

    std::cout << to_string(baz) << "\n";
    std::cout << to_string(foo) << "\n";
    std::cout << to_string(bar) << "\n";
}

std::string stringify(int i)
{
    return "\"" + std::to_string(i) + "\"";
}

bool iseven(int i)
{
    return (i % 2) == 0;
}

TEST(LinkedListTest, FunctionalTesting)
{
    LinkedList<int> foo;

    for (auto i = 0; i < 10; ++i)
    {
        foo.append(i);
    }

    auto filtered = list_filter<int>(foo, iseven);
    EXPECT_EQ(to_string(*filtered), "[0, 2, 4, 6, 8]");

    // Trailing type on the lambda isn't strictly necessary,
    filtered = list_filter<int>(foo, [](auto i) -> bool
                                    { return i % 2; });
    EXPECT_EQ(to_string(*filtered), "[1, 3, 5, 7, 9]");

    filtered = list_filter<int>(foo, [](auto i)
                                    { return i % 2; });
    EXPECT_EQ(to_string(*filtered), "[1, 3, 5, 7, 9]");

    auto baz = list_map<std::string, int>(foo,
                                          stringify);
    EXPECT_EQ(to_string(*baz), "[\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\"]");



    auto bar = list_map<int, int>(foo,
                                  [](auto x)
                                  { return x + 1; });
    EXPECT_EQ(to_string(*bar), "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]");

    const auto incr = 10;
    bar = list_map<int, int>(foo,
                                  [](auto x)
                                  { return x + incr; });
    EXPECT_EQ(to_string(*bar), "[10, 11, 12, 13, 14, 15, 16, 17, 18, 19]");

    auto garplay = list_reduce<int, int>(foo, [](auto x, auto y)
                                         { return x + y; }, 0);
    EXPECT_EQ(garplay, 45);
}