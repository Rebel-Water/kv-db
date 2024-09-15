#pragma once
#include "Type.hpp"
#include <vector>
#include <string>

class Util
{
    public:
    static std::vector<byte> ToByteVector(const std::string &str)
    {
        return std::move(std::vector<byte>(str.begin(), str.end())); // although rvo will fix it...
    }                                                               //
                                                                    //
    static std::string ToString(const std::vector<byte> &vec)       //
    {                                                               //
        return std::move(std::string(vec.begin(), vec.end()));      // but if undef rvo, this also help
    }
};