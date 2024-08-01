#pragma once
#include<vector>
#include "Type.hpp"
#include <memory>
#include <zlib.h>

enum LogRecordType {
    LogRecordNormal, LogRecordDeleted, LogRecordTxnFinsihed
};

struct LogRecordHeader {
    LogRecordHeader() {}
    LogRecordHeader(uint32 crc, LogRecordType type, uint32 keySize, uint32 valueSize, uint32 headerSize)
        : crc(crc), recordType(type), keySize(keySize), valueSize(valueSize), headerSize(headerSize)
     {}
    uint32 crc;
    LogRecordType recordType;
    uint32 keySize;
    uint32 valueSize;
    uint32 headerSize;
};

struct LogRecord {
    public:
    LogRecord() {}
    LogRecord(const LogRecord& logRecord) : Key(logRecord.Key), Value(logRecord.Value), Type(logRecord.Type), Size(logRecord.Size) {}
    LogRecord(const std::vector<byte>& key, LogRecordType type) 
        : Key(key), Value({}), Type(type) {}
    LogRecord(const std::vector<byte>& key, const std::vector<byte>& value, LogRecordType type) 
        : Key(key), Value(value), Type(type) {}
    LogRecord(const std::vector<byte>& key, const std::vector<byte>& value) 
        : Key(key), Value(value), Type(LogRecordNormal) {}

    uint32 GetLogRecordCRC(std::vector<byte>& header) {
        uint32 crc = crc32(CRC_DEFAULT, header.data(), header.size());
        crc = crc32(crc, this->Key.data(), this->Key.size());
        if(this->Value.size() != 0)
            crc = crc32(crc, this->Value.data(), this->Value.size());
        return crc;
    }

    std::vector<byte> Key;
    std::vector<byte> Value;
    LogRecordType Type;
    int64 Size;
};

struct LogRecordPos {
    LogRecordPos() : isEmpty(true), Size(0) {}
    LogRecordPos(const LogRecordPos& pos) : Fid(pos.Fid), Offset(pos.Offset), Size(pos.Size), isEmpty(pos.isEmpty){}
    LogRecordPos(uint32 Fid, int64 Offset) : Fid(Fid), Offset(Offset), isEmpty(false) {}
    LogRecordPos(uint32 Fid, int64 Offset, uint32 Size) : Fid(Fid), Offset(Offset), Size(Size), isEmpty(false) {}
    uint32 Fid;
    int64 Offset;
    uint32 Size;
    bool isEmpty;
};

struct TransactionRecord {
    TransactionRecord(std::unique_ptr<LogRecord> Record, LogRecordPos Pos) : Record(std::move(Record)), Pos(Pos) {}
    std::unique_ptr<LogRecord> Record;
    LogRecordPos Pos;
};