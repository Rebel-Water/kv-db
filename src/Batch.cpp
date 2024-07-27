#include <gtest/gtest.h>
#include "Batch.hpp" 
#include "Util.hpp"
#include "DB.hpp"
#include "Code.hpp"
#include <tuple>

std::vector<byte> txnFinKey = Util::ToByteVector("txn-fin");

void WriteBatch::Put(const std::vector<byte>& key, const std::vector<byte>& value) {
    if(key.size() == 0)
        throw std::runtime_error("WriteBatch::Put Key Is Empty");
    std::lock_guard lock(this->mutex);
    this->pendingWrites[Util::ToString(key)] = std::make_unique<LogRecord>(key, value, LogRecordNormal);
}

void WriteBatch::Delete(const std::vector<byte>& key) {
    if(key.size() == 0) 
        throw std::runtime_error("WriteBatch::Delete Key Is Empty");
    std::lock_guard lock(this->mutex);
    auto pos = this->db->index->Get(key);
    std::string key_str = Util::ToString(key);
    
    if(pos == nullptr) {
        if(this->pendingWrites.find(key_str) != this->pendingWrites.end())
            this->pendingWrites.erase(key_str);
        return ;
    }
    this->pendingWrites[key_str] = std::make_unique<LogRecord>(key, LogRecordDeleted);
}

void WriteBatch::Commit() {
    std::lock_guard lock(this->mutex);
    if(this->pendingWrites.size() == 0)
        return ;
    if(this->pendingWrites.size() > this->option.MaxBatchNum)
        throw std::runtime_error("WriteBatch::Commit Key Is Empty");
    uint64 seqNo = this->db->seqNo.fetch_add(1) + 1;
    std::unordered_map<std::string, LogRecordPos> positions;
    // int cnt = 0;
    for(auto& [_, record] : this->pendingWrites) {
        auto pos = this->db->AppendLogRecord(std::make_unique<LogRecord>(
            logRecordKeyWithSeq(record->Key, seqNo), record->Value, record->Type
        )); // in our appendlogrecord, call it should hold write lock
        positions[Util::ToString(record->Key)] = pos;

        //if(++cnt == 100000)
        //    return ;
    }

    auto finishRecord = std::make_unique<LogRecord>(logRecordKeyWithSeq(txnFinKey, seqNo), LogRecordTxnFinsihed);
    this->db->AppendLogRecord(std::move(finishRecord));

    if(this->option.SyncWrite && this->db->activeFile) 
        this->db->activeFile->Sync();

    for(auto& [_, record] : this->pendingWrites) {
        auto pos = positions[Util::ToString(record->Key)];
        if(record->Type == LogRecordNormal)
            this->db->index->Put(record->Key, pos);
        if(record->Type == LogRecordDeleted)
            this->db->index->Delete(record->Key);
    }
    this->pendingWrites.clear();
}