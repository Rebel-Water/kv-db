#pragma once
#include <shared_mutex>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Type.hpp"
#include "LogRecord.hpp"
#include "DataFile.hpp"
#include "Options.hpp"
#include "Indexer.hpp"

class Iterator;
class Indexer;
class DB {
    public:
    using FoldFn = std::function<bool(const std::vector<byte>&, const std::vector<byte>&)>;
    DB(const Options& option);
    ~DB();

    std::shared_mutex RWMutex;
    Options option;
    std::unique_ptr<Indexer> index;
    std::shared_ptr<DataFile> activeFile;
    std::vector<int> fileIds;
    std::unordered_map<uint32, std::shared_ptr<DataFile>> olderFiles;

    void Fold(FoldFn fn);
    std::vector<std::vector<byte>> ListKey();

    void Sync();
    void Close();
    std::vector<byte> Get(const std::vector<byte>& key);
    void Put(const std::vector<byte>& key, const std::vector<byte>& value);
    void Delete(const std::vector<byte>& key);

    std::unique_ptr<LogRecord> ReadLogRecord(int64 offset, std::shared_ptr<DataFile> datafile);
    LogRecordPos AppendLogRecord(std::unique_ptr<LogRecord> logRecord);

    std::unique_ptr<Iterator> NewIterator(IteratorOptions option);

    std::vector<byte> getValueByPosition(const LogRecordPos& pos);
    void SetActiveDataFile();
    void checkOptions(const Options& option);
    void LoadDataFiles();
    void LoadIndexFromDataFiles();

};

