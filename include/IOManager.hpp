#pragma once
#include "Type.hpp"
#include <vector>

class IOManager {
public:
    virtual std::streamsize Read(std::vector<byte>& buf, int64 offset) = 0;
    virtual std::streamsize Write(const std::vector<byte>& buf) = 0;
    virtual void Sync() = 0;
    virtual void Close() = 0;
    virtual std::streamsize Size() = 0;
};