#pragma once
#include <shared_mutex>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Type.hpp"
#include "LogRecord.hpp"
#include "DataFile.hpp"
#include "Options.hpp"
#include "Indexer.hpp"

class DB {
    std::shared_mutex RWMutex;
    Options option;
    std::unique_ptr<Indexer> index;
    std::unique_ptr<DataFile> activeFile;
    std::unordered_map<uint32, std::unique_ptr<DataFile>> olderFiles;

    std::vector<byte> Get(const std::vector<byte>& key);
    void Put(std::vector<byte>& key, std::vector<byte>& value);
    std::unique_ptr<LogRecordPos> AppendLogRecord(std::unique_ptr<LogRecord> logRecord);
    void SetActiveDataFile();
};

