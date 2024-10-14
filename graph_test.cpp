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
        auto g = Graph<int>::create();
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

        for(auto step : DijkstraTraversal<int>(g, 0)) {
            EXPECT_TRUE(step->current->name == i);
            EXPECT_TRUE(step->distance == float(i));
            if(i != 0) {
                EXPECT_TRUE(step->previous->name == (i-1));
            } else {
                EXPECT_TRUE(step->previous == nullptr);
            }
            i++;
        }
        for(i = 0; i < 10; ++i) {
            for(auto j = 0; j < 10; ++j) {
                if((i + 1) % 10 != j) {
                    g->create_link(i,j,11);
                }
            }
        }
        i = 0;
        for(auto step : DijkstraTraversal<int>(g, 0)) {
            EXPECT_TRUE(step->current->name == i);
            EXPECT_TRUE(step->distance == float(i));
            i++;
        }
    }
}