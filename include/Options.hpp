#pragma once
#include <string>
#include "Type.hpp"

class Options {
    public:
    std::string DirPath;
    int64 DataFileSize;
    bool SyncWrite;
    IndexType Type;

};