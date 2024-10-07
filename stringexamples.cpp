#include <string>

#include "stringexamples.hpp"

#include <ctype.h>
#include <algorithm>

std::string toupper(std::string s){
    for(auto &c: s){
        c = toupper(c);
    }
    return s;
}

std::string reverse(std::string s){
    reverse(s.begin(), s.end());
    return s;
}

std::string stripl(std::string s){
    auto index = 0;
    for (auto c : s){
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            break;
        }
        index++;
    }
    return s.substr(index);
}

std::string stripr(std::string s){
    auto len = s.length();
    for (auto citerator = s.rbegin(); citerator != s.rend(); ++citerator){
        if(*citerator != ' ' && *citerator != '\t' && *citerator != '\n' && *citerator != '\r') {
            break;
        }
        len--;
    }
    return s.substr(0, len);
}