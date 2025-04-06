#include <string>

#include "stringexamples.hpp"
#include "confuzzle.h"

#include <ctype.h>
#include <algorithm>
#include <ranges>

std::string toupper(std::string s)
{
    for (auto &c : s)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        c = toupper(c);
#pragma GCC diagnostic pop
    }
    return s;
}

std::string reverse(std::string s)
{
    reverse(s.begin(), s.end());
    return s;
}

std::string stripl(std::string s)
{
    auto index = 0;
    for (auto c : s)
    {
        if (!isspace(c))
        {
            break;
        }
        index++;
    }
    return s.substr(index);
}

/*

std::string stripr(std::string s){
    auto len = s.length();
    for (auto citerator = s.rbegin(); citerator != s.rend(); ++citerator){
        if(!isspace(*citerator)) {
            break;
        }
        len--;
    }
    return s.substr(0, len);
}


*/

std::string stripr(std::string s)
{
    auto len = s.length();
    for (auto c : s | std::views::reverse)
    {
        if (!isspace(c))
        {
            break;
        }
        len--;
    }
    return s.substr(0, len);
}
