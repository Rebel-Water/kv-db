#include "Batch.hpp"
#include "Util.hpp"
#include "DB.hpp"
#include "Code.hpp"
#include <tuple>

const std::vector<byte> WriteBatch::txnFinKey = Util::ToByteVector("txn-fin");

std::vector<byte> WriteBatch::logRecordKeyWithSeq(const std::vector<byte> &key, uint64 seqNo)
{
    auto seq = std::vector<byte>(MaxVarintLen64);
    int n = Code::PutVarint(seq, 0, seqNo);
    auto encodeKey = std::vector<byte>(key.size() + n);
    std::copy(seq.begin(), seq.begin() + n, encodeKey.begin());
    std::copy(key.begin(), key.end(), encodeKey.begin() + n);
    return std::move(encodeKey);
};

std::pair<std::vector<byte>, int> WriteBatch::parseLogRecordKey(const std::vector<byte> &key)
{
    int n = 0;
    int seqNo = Code::GetVarint(key, 0, &n);
    std::vector<byte> buf(key.begin() + n, key.end());
    return std::make_pair(std::move(buf), seqNo);
};

void WriteBatch::Put(const std::vector<byte> &key, const std::vector<byte> &value)
{
    if (key.size() == 0)
        throw std::runtime_error("WriteBatch::Put Key Is Empty");
    std::lock_guard lock(this->mutex);
    this->pendingWrites[Util::ToString(key)] = std::make_unique<LogRecord>(key, value, LogRecordType::LogRecordNormal);
}

void WriteBatch::Delete(const std::vector<byte> &key)
{
    if (key.size() == 0)
        throw std::runtime_error("WriteBatch::Delete Key Is Empty");
    std::lock_guard lock(this->mutex);
    auto pos = this->db->index->Get(key);
    std::string key_str = Util::ToString(key);

    if (!pos.has_value())
    {
        if (this->pendingWrites.find(key_str) != this->pendingWrites.end())
            this->pendingWrites.erase(key_str);
        return;
    }
    this->pendingWrites[key_str] = std::make_unique<LogRecord>(key, LogRecordType::LogRecordDeleted);
}

void WriteBatch::Put(const std::string &key, const std::string &value)
{
    this->Put(Util::ToByteVector(key), Util::ToByteVector(value));
}

void WriteBatch::Delete(const std::string &key)
{
    this->Delete(Util::ToByteVector(key));
}

void WriteBatch::Commit()
{
    std::lock_guard lock(this->mutex);
    if (this->pendingWrites.size() == 0)
        return;
    if (this->pendingWrites.size() > this->option.MaxBatchNum)
        throw std::runtime_error("WriteBatch::Commit Key Is Empty");
    uint64 seqNo = this->db->seqNo.fetch_add(1) + 1;
    std::unordered_map<std::string, LogRecordPos> positions;
    
    for (auto &[_, record] : this->pendingWrites)
    {
        LogRecord logrecord(logRecordKeyWithSeq(record->Key, seqNo), record->Value, record->Type);
        auto pos = this->db->AppendLogRecord(logrecord); // in our appendlogrecord, call it should hold write lock
        positions[Util::ToString(record->Key)] = pos;
    }

    LogRecord finishRecord(logRecordKeyWithSeq(txnFinKey, seqNo), LogRecordType::LogRecordTxnFinsihed);
    this->db->AppendLogRecord(finishRecord);

    if (this->option.SyncWrite && this->db->activeFile)
        this->db->activeFile->Sync();

    for (auto &[_, record] : this->pendingWrites)
    {
        auto pos = positions[Util::ToString(record->Key)];
        if (record->Type == LogRecordType::LogRecordNormal)
        {
            auto ret = this->db->index->Put(record->Key, pos);
            this->db->reclaimableSize += ret->Size;
        }
        if (record->Type == LogRecordType::LogRecordDeleted)
        {
            auto ret = this->db->index->Delete(record->Key);
            this->db->reclaimableSize += ret->Size;
        }
    }
    this->pendingWrites.clear();
}