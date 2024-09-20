#pragma once
#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Options.hpp"
#include "Util.hpp"
#include "Code.hpp"

class DB;
class LogRecord;

class WriteBatch
{
public:
    static const int NonTransactionSeqNo = 0;
    static const std::vector<byte> txnFinKey;

    static std::vector<byte> logRecordKeyWithSeq(const std::vector<byte> &key, uint64 seqNo);
    static std::pair<std::vector<byte>, int> parseLogRecordKey(const std::vector<byte> &key);

    WriteBatch(const WriteBatchOptions &option, DB *db) : option(option), db(db) {}

    void Put(const std::vector<byte> &key, const std::vector<byte> &value);
    void Delete(const std::vector<byte> &key);

    void Put(const std::string &key, const std::string &value);
    void Delete(const std::string &key);
    void Commit();

    #ifdef GTEST
    inline auto getDB() { return db; }
    #endif

private:
    WriteBatchOptions option;
    DB *db;
    std::mutex mutex;
    std::unordered_map<std::string, std::unique_ptr<LogRecord>> pendingWrites;
    friend class DB;
    
};