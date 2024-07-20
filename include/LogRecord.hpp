#pragma once
#include<vector>
#include "Type.hpp"

enum LogRecordType {
    LogRecordNormal, LogRecordDeleted, LogRecordTxnFinsihed
};

class LogRecord {
    public:
    LogRecord() {}
    LogRecord(const LogRecord& logRecord) : Key(logRecord.Key), Value(logRecord.Value), Type(logRecord.Type) {}
    LogRecord(const std::vector<byte>& key, const std::vector<byte>& value, LogRecordType type) 
        : Key(key), Value(value), Type(type) {}
    std::vector<byte> Key;
    std::vector<byte> Value;
    LogRecordType Type;
};

class LogRecordPos {
    public:
    LogRecordPos(uint32 Fid, int64 Offset) : Fid(Fid), Offset(Offset){}
    LogRecordPos(uint32 Fid, int64 Offset, uint32 Size) {}
    uint32 Fid;
    int64 Offset;
    uint32 Size;
};

