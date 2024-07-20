#pragma once
#include "Type.hpp"
#include <vector>

class IOManager {
    virtual int Read(std::vector<byte>& buf, int64 length);
    virtual int Write(const std::vector<byte>& buf);
    void Sync();
    void Close();
};