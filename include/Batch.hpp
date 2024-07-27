#pragma once
#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Options.hpp"
#include "Code.hpp"

class DB;
class LogRecord;

const int NonTransactionSeqNo = 0;

class WriteBatch
{
public:
    static std::vector<byte> logRecordKeyWithSeq(const std::vector<byte> &key, uint64 seqNo)
    {
        auto seq = std::vector<byte>(MaxVarintLen64);
        int n = Code::PutVarint(seq, 0, seqNo);
        auto encodeKey = std::vector<byte>(key.size() + n);
        std::copy(seq.begin(), seq.begin() + n, encodeKey.begin());
        std::copy(key.begin(), key.end(), encodeKey.begin() + n);
        return encodeKey;
    };

    static std::pair<std::vector<byte>, int> parseLogRecordKey(const std::vector<byte> &key)
    {
        int n = 0;
        int seqNo = Code::GetVarint(key, 0, &n);
        std::vector<byte> buf(key.begin() + n, key.end());
        return std::make_pair(buf, seqNo);
    };

    WriteBatch(const WriteBatchOptions &option, DB *db) : option(option), db(db) {}

    void Put(const std::vector<byte> &key, const std::vector<byte> &value);
    void Delete(const std::vector<byte> &key);
    void Commit();

    WriteBatchOptions option;
    DB *db;
    std::mutex mutex;
    std::unordered_map<std::string, std::unique_ptr<LogRecord>> pendingWrites;
};