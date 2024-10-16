#include <gtest/gtest.h>
#include <string>
#include "stringexamples.hpp"
#include <fstream>

// Demonstrate opening a file for reading
TEST(FileTest, BasicLoadingGetline)
{
    // This is being run in build but the actual file is outside, so
    // we do ../ on loading
    std::ifstream input{"../testfile.txt"};
    if (!input.is_open())
    {
        ASSERT_TRUE(false);
    }
    std::string s;
    while (!input.eof())
    {
        // Getline gets the string but EXCLUDES the newline
        getline(input, s);
        std::cout << "getline Got string \"" << s << "\"\n";
    }
}

TEST(FileTest, BasicLoadingGetlineSmartpointer)
{
    auto input = std::make_shared<std::ifstream>("../testfile.txt");
    if (!input->is_open())
    {
        ASSERT_TRUE(false);
    }
    std::string s;
    while (!input->eof())
    {
        // Getline gets the string but EXCLUDES the newline
        getline(*input, s);
        std::cout << "getline Got string \"" << s << "\"\n";
    }
}

TEST(FileTest, OperatorInTest)
{
    std::ifstream input{"../testfile.txt"};
    if (!input.is_open())
    {
        ASSERT_TRUE(false);
    }
    std::string s;
    while (!input.eof())
    {
        input >> s;
        std::cout << "<< Got string \"" << s << "\"\n";
    }
}

TEST(FileTest, OperatorInBetter)
{
    // Here is an example of doing an fstream, where we specify it is binary
    // (which doesn't affect getline from working) and that we are reading the file
    // showing how we can OR two flags together
    std::fstream input{"../testfile.txt", std::istream::binary | std::istream::in};
    if (!input.is_open())
    {
        ASSERT_TRUE(false);
    }
    std::string s;
    while (!input.eof())
    {
        getline(input, s);
        std::istringstream ibuf(s);
        int i;
        ibuf >> i;
        if (ibuf)
        {
            std::cout << "<< Got integer \"" << i << "\"\n";
            std::cout.setf(std::ios_base::hex, std::ios_base::basefield);
            std::cout.setf(std::ios_base::showbase);
            std::cout << "<< As hex it is \"" << i << "\"\n";

            std::cout.unsetf(std::ios_base::hex);
            std::cout.unsetf(std::ios_base::showbase);
            std::cout << "<< and back at default \"" << i << "\"\n";

            // Can also shortcut
            std::cout << std::hex << "<< As hex it is \"" << i << "\"\n";
            std::cout << std::dec << "<< and back at default \"" << i << "\"\n";
        }
    }
}
