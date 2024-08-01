#pragma once
#include <cstdint>
#include <string>

using byte = uint8_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

enum IOType{
    FileIOType,
    MmapType
};

enum IndexType {
    BTree_Index
};

enum DataFileType {
    DataNormalFile,
    HintFile,
    MergeFinishFile
};

const std::string DataFileNameSuffix    = ".txt";
const std::string HintFileName          = "hint-index";
const std::string MergeFinishedFileName = "merge-finished";
const std::string SeqNoFileName         = "seq-no";

const int MaxVarintLen32 = 5;
const int MaxVarintLen64 = 10;

const int MaxLogRecordHeaderSize = MaxVarintLen32 * 2 + 5;

const int IndexLogRecordType = 4;

const uint32 CRC_DEFAULT = 0;

const std::string defaultPath = "/home/ace/kv/data/";
const int defaultDataFileSize = 256 * 1024 * 1024;
const int DefaultMaxBatchNum = 10000;