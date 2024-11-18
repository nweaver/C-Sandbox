#include <gtest/gtest.h>
#include <string>
#include "workqueue.hpp"
#include <thread>
#include <ranges>
#include <cstdlib>
#include <chrono>

// Demonstrate some basic assertions.
TEST(WorkQueue, BasicTest)
{
    // auto rng = std::default_random_engine();

    for (auto y : std::views::iota(0, 20))
    {
        WorkQueue<int> w;

        std::jthread j([&]()
                       {
                        std::cout << ".";
                        std::cout.flush();
                        for (auto i : std::views::iota(0, 100))
                        {
                            std::chrono::milliseconds ms{std::rand() % 3};
                            std::this_thread::sleep_for(ms);
                            EXPECT_EQ(w.get(), i);
                        } 
        });
        (void)y;
        for (auto i : std::views::iota(0, 100))
        {
            std::chrono::milliseconds ms{std::rand() % 3};
            std::this_thread::sleep_for(ms);
            w.put(i);
        }
    }
    std::cout << "\n";
}
