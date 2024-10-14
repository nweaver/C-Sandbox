#include <gtest/gtest.h>
#include <string>
#include "graph.hpp"
#include <random>

// Demonstrate some basic assertions.
TEST(GraphTest, Comprehensive)
{
    auto array = std::vector<int>(10);
    auto array2 = std::vector<int>(10);

    int i;
    for (i = 0; i < 10; ++i)
    {
        array[i] = i;
        array2[i] = i;
    }
    auto rng = std::default_random_engine{};
    for (auto k = 0; k < 10; ++k)
    {
        auto g = std::make_shared<graph<int>>();
        std::shuffle(std::begin(array), std::end(array), rng);
        for (i = 0; i < 10; ++i)
        {
            g->create_node(array[i]);
        }
        std::shuffle(std::begin(array), std::end(array), rng);
        for (i = 0; i < 10; ++i)
        {
            g->create_link(array[i], (array[i] + 1) % 10, 1.0);
            EXPECT_THROW(g->create_link(array[i], (array[i] + 1) % 10, 32.3), std::domain_error);
        }
        i = 0;
    }
}