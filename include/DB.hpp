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
    DB(Options option);

    std::shared_mutex RWMutex;
    Options option;
    std::unique_ptr<Indexer> index;
    std::shared_ptr<DataFile> activeFile;
    std::vector<int> fileIds;
    std::unordered_map<uint32, std::shared_ptr<DataFile>> olderFiles;

    std::unique_ptr<DB> Open(Options option);
    std::vector<byte> Get(const std::vector<byte>& key);
    void Put(std::vector<byte>& key, std::vector<byte>& value);
    void Delete(std::vector<byte>& key);

    std::unique_ptr<LogRecord> ReadLogRecord(int64 offset, std::shared_ptr<DataFile> datafile);
    std::unique_ptr<LogRecordPos> AppendLogRecord(std::unique_ptr<LogRecord> logRecord);

    void SetActiveDataFile();
    void checkOptions(Options option);
    void LoadDataFiles();
    void LoadIndexFromDataFiles();
};

