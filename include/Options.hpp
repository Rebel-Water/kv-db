#pragma once
#include <string>
#include "Type.hpp"

struct Options {
    Options(std::string DirPath = defaultPath, int64 DataFileSize = defaultDataFileSize, bool SyncWrite = false, IndexType Type = IndexType::BTree_Index, bool isOpenMergeDetect = false, float DataFileMergeRatio = 0.3)  
        : DirPath(DirPath), DataFileSize(DataFileSize), SyncWrite(SyncWrite), Type(Type), BytesPerSync(0), isOpenMergeDetect(isOpenMergeDetect), DataFileMergeRatio(DataFileMergeRatio)
    {}
    Options(const Options& option) 
        : DirPath(option.DirPath), DataFileSize(option.DataFileSize), SyncWrite(option.SyncWrite), Type(option.Type)
    {}
    void SetBytesPerSync(uint bytes) { BytesPerSync = bytes; }
    std::string DirPath;
    int64 DataFileSize;
    bool SyncWrite;
    uint BytesPerSync;
    float DataFileMergeRatio;
    IndexType Type;
    bool isOpenMergeDetect;
};

struct IteratorOptions {
    IteratorOptions(const std::vector<byte>& Prefix = {}, bool Reverse = false) 
        : Prefix(Prefix), Reverse(Reverse)
    {}
    // skiptoNext jump to next fit Prefix position
    std::vector<byte> Prefix;
    bool Reverse;
};

struct WriteBatchOptions {
    WriteBatchOptions(uint MaxBatchNum = DefaultMaxBatchNum, bool SyncWrite = true) : MaxBatchNum(MaxBatchNum), SyncWrite(SyncWrite) {}
    WriteBatchOptions(const WriteBatchOptions& opt) : MaxBatchNum(opt.MaxBatchNum), SyncWrite(opt.SyncWrite) {}
    uint MaxBatchNum;
    bool SyncWrite;
};