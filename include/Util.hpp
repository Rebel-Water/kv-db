#pragma once
#include "Type.hpp"
#include <vector>
#include <string>

std::vector<byte> ToByteVector(const std::string &str) {
    return std::vector<byte>(str.begin(), str.end());
}

std::string ToString(const std::vector<byte>& vec) {
    return std::string(vec.begin(), vec.end());
}