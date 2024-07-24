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
    public:
    DB(const Options& option);

    // std::shared_mutex RWMutex;
    Options option;
    std::unique_ptr<Indexer> index;
    std::shared_ptr<DataFile> activeFile;
    std::vector<int> fileIds;
    std::unordered_map<uint32, std::shared_ptr<DataFile>> olderFiles;

    std::vector<byte> Get(const std::vector<byte>& key);
    void Put(const std::vector<byte>& key, const std::vector<byte>& value);
    void Delete(const std::vector<byte>& key);

    std::unique_ptr<LogRecord> ReadLogRecord(int64 offset, std::shared_ptr<DataFile> datafile);
    std::unique_ptr<LogRecordPos> AppendLogRecord(std::unique_ptr<LogRecord> logRecord);

    void SetActiveDataFile();
    void checkOptions(const Options& option);
    void LoadDataFiles();
    void LoadIndexFromDataFiles();
};

