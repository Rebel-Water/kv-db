#pragma once
#include<vector>
#include "Type.hpp"

enum LogRecordType {
    LogRecordNormal, LogRecordDeleted, LogRecordTxnFinsihed
};

class LogRecordHeader {
    public:
    uint32 crc;
    LogRecordType recordType;
    uint32 keySize;
    uint32 valueSize;
    uint32 headerSize;
};

class LogRecord {
    public:
    LogRecord() {}
    LogRecord(const LogRecord& logRecord) : Key(logRecord.Key), Value(logRecord.Value), Type(logRecord.Type) {}
    LogRecord(const std::vector<byte>& key, LogRecordType type) 
        : Key(key), Type(type) {}
    LogRecord(const std::vector<byte>& key, const std::vector<byte>& value, LogRecordType type) 
        : Key(key), Value(value), Type(type) {}

    uint32 getLogRecordCRC(std::vector<byte>& header) {
        return 0;
    }


    std::vector<byte> Key;
    std::vector<byte> Value;
    LogRecordType Type;
    int64 Size;
};

class LogRecordPos {
    public:
    LogRecordPos(uint32 Fid, int64 Offset) : Fid(Fid), Offset(Offset){}
    LogRecordPos(uint32 Fid, int64 Offset, uint32 Size) {}
    uint32 Fid;
    int64 Offset;
    uint32 Size;
};

