#pragma once
#include <memory>
#include "LogRecord.hpp"

const int MaxVarintLen32 = 5;

const int MaxLogRecordHeaderSize = MaxVarintLen32 * 2 + 5;

class Code
{
public:
    static std::vector<byte> EncodeLogRecord(std::unique_ptr<LogRecord> logRecord)
    {
        return std::vector<byte>();
    }

    static std::unique_ptr<LogRecordHeader> DecodeLogRecord(std::vector<byte> buf)
    {
        return nullptr;
    }


    static std::vector<byte> varintEncode(uint64_t val)
    {
        std::vector<byte> result;
        while (val >= 0x80)
        {
            result.emplace_back(0x80 | (val & 0x7f));
            val >>= 7;
        }
        // The last byte always < 0x80
        result.emplace_back(byte(val));
        return result;
    }

    static std::string varintEncodeStr(uint64_t val)
    {
        byte buf[16];
        size_t size = varintEncodeBuf(&buf[0], sizeof(buf), val);
        // INVARIANT_D(size < sizeof(buf));
        return std::string(reinterpret_cast<char *>(&buf[0]), size);
    }

    static size_t varintEncodeBuf(uint8_t *buf, size_t bufsize, uint64_t val)
    {
        size_t size = 0;
        while (val >= 0x80)
        {
            buf[size++] = (0x80 | (val & 0x7f));
            val >>= 7;
        }
        // The last byte always < 0x80
        buf[size++] = (uint8_t)val;

        // INVARIANT_D(size <= bufsize);
        return size;
    }
};