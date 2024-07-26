#pragma once
#include <string>
#include "Type.hpp"

const std::string defaultPath = "/home/ace/kv/";
const int defaultDataFileSize = 256 * 1024 * 1024;

class Options {
    public:
    // Options() {}
    // modify!!!
    Options(std::string DirPath = defaultPath, int64 DataFileSize = defaultDataFileSize, bool SyncWrite = false, IndexType Type = BTree_Index)  
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

class IteratorOptions {
    public:
    IteratorOptions(const std::vector<byte>& Prefix = {}, bool Reverse = false) 
        : Prefix(Prefix), Reverse(Reverse)
    {}
    // skiptoNext jump to next fit Prefix position
    std::vector<byte> Prefix;
    bool Reverse;
};