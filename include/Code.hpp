#pragma once
#include <memory>
#include <cstring>
#include "LogRecord.hpp"
#include <zlib.h>
#include <optional>

class Code
{
public:
    static std::vector<byte> EncodeLogRecord(LogRecord &logRecord);

    static std::optional<LogRecordHeader> DecodeLogRecord(const std::vector<byte> &buf);

    static std::vector<byte> EncodeLogRecordPos(LogRecordPos &pos);

    static LogRecordPos DecodeLogRecordPos(const std::vector<byte> &buf);

    static int PutVarint(std::vector<byte> &buf, int index, int val);
    static int GetVarint(const std::vector<byte> &buf, int index, int *length);

    static int PutUint64(std::vector<byte> &buf, int index, int val);
    static int GetUint64(const std::vector<byte> &buf, int index);

    static double FloatFromBytes(const std::vector<byte> &val);
    static std::vector<byte> Float64ToBytes(double val);
};