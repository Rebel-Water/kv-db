#pragma once
#include <string>
#include "Type.hpp"

class Options {
    public:
    Options() {}
    Options(std::string DirPath, int64 DataFileSize, bool SyncWrite, IndexType Type)  
        : DirPath(DirPath), DataFileSize(DataFileSize), SyncWrite(SyncWrite), Type(Type)
    {}
    Options(const Options& option) 
        : DirPath(option.DirPath), DataFileSize(option.DataFileSize), SyncWrite(option.SyncWrite), Type(option.Type)
    {}
    std::string DirPath;
    int64 DataFileSize;
    bool SyncWrite;
    IndexType Type;

};