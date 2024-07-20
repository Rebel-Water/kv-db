#pragma once
#include<vector>
#include "Type.hpp"

enum LogRecordType {
    LogRecordNormal, LogRecordDeleted, LogRecordTxnFinsihed
};

class LogRecordPos {
    public:
    LogRecordPos(uint32 Fid, int64 Offset) : Fid(Fid), Offset(Offset){}
    LogRecordPos(uint32 Fid, int64 Offset, uint32 Size) {}
    uint32 Fid;
    int64 Offset;
    uint32 Size;
};

class LogRecord {
    std::vector<byte> Key;
    std::vector<byte> Value;
    LogRecordType Type;
};