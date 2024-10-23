#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <ranges>
#include <algorithm>
#include <functional>

std::pair<int, float> foo()
{
    return {2, 3.14159};
}
std::tuple<int, float, std::string> bar()
{
    return {13, 7.8, "this is a string"};
}

TEST(TupleTests, BasicDestructuring)
{
    auto [a, b] = foo();
    auto [c, d, e] = bar();
    std::cout << a << " " << b << "\n";
    std::cout << c << " " << d << " " << e << "\n";
}

TEST(TupleTests, ManualDestructuring)
{
    auto baz = foo();
    std::cout << baz.first << " " << baz.second << "\n";
    // Pairs have the field names .first, and .second that can be
    // accessed
    auto garplay = bar();
    std::cout << std::get<0>(garplay) << " " << std::get<1>(garplay) << " " << std::get<2>(garplay) << "\n";
}

TEST(TupleTests, PipeSorting)
{
    std::vector<std::string> test = {"foo", "bar", "baz", "garplay"};
    for (auto s : test)
    {
        std::cout << s << ", ";
    }
    std::cout << "\n";
    std::sort(test.begin(), test.end());
    for (auto s : test)
    {
        std::cout << s << ", ";
    }
    std::cout << "\n";
    for (auto s : test | std::views::reverse)
    {
        std::cout << s << ", ";
    }
    std::cout << "\n";
    for (auto s : test | std::views::filter([](std::string s)
                                            { return s == "foo"; }))
    {
        std::cout << s << ", ";
    }
    std::cout << "\n";

    for (auto s : test | std::views::transform([](std::string s)
                                               { return s.length(); }))
    {
        std::cout << s << ", ";
    }
    std::cout << "\n";

    for (auto i : std::views::iota(0, 10))
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    for (auto i : std::views::iota(0) | std::views::take(10))
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    for (auto i : std::views::iota(0) | std::views::take(10) | std::views::reverse)
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    for (auto i : std::views::iota(0) |
                      std::views::filter([](auto i)
                                         { return i % 2 == 0; }) |
                      std::views::take(10) | std::views::reverse)
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    auto allEven = std::views::iota(0) | std::views::filter([](auto i)
                                                            { return i % 2 == 0; });
    for (auto i : allEven | std::views::take(10))
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    for (auto i : allEven | std::views::drop(10) | std::views::take(10))
    {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    std::map<std::string, int> testmap = { {"fubar", 42 } };
    for(auto i : std::views::iota(0, 10) | std::views::reverse ) {
        std::stringstream s;
        s << "The value is " << i;
        testmap[s.str()] = i;
    }
    for(auto [key, value] : testmap){
        std::cerr << "Key: " << key << " Value: " << value << "\n";
    }

    std::multimap<int, char> example = {{1, 'a'}, {2, 'b'}, {2, 'c'}, {3, 'x'}};
    for(auto search = example.find(2); search != example.end(); ++search){
        auto [key, value] = *search;
        std::cout << "Key : " << key << " Value: " << value << "\n";
    }

}