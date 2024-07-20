#pragma once
#include "Type.hpp"
#include "IOManager.hpp"
#include <string>
#include <memory>

const std::string DataFileNameSuffix    = ".data";
const std::string HintFileName          = "hint-index";
const std::string MergeFinishedFileName = "merge-finished";
const std::string SeqNoFileName         = "seq-no";

class DataFile {
public:
    uint32 FileId;
    int64 WriteOff;
    IOManager IoManager;
    std::unique_ptr<DataFile> OpenDataFile(std::string path, uint32 fileId) {
        return nullptr;
    }
    void Sync() {
        return ;
    }

    void Write(const std::vector<byte>& buf) {
        return ;
    }

    std::unique_ptr<LogRecord> ReadLogRecord(int64 offset) {
        return nullptr;
    }
};