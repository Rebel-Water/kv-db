#pragma once
#include <cstdint>
#include <string>

using byte = uint8_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

enum IndexType {
    BTree_Index
};

const std::string DataFileNameSuffix    = ".data";
const std::string HintFileName          = "hint-index";
const std::string MergeFinishedFileName = "merge-finished";
const std::string SeqNoFileName         = "seq-no";