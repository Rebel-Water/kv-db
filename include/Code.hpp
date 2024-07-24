#pragma once
#include <memory>
#include <cstring>
#include "LogRecord.hpp"
#include <zlib.h>

class Code
{
public:
    static std::vector<byte> EncodeLogRecord(std::unique_ptr<LogRecord> logRecord)
    {
        std::vector<byte> header(MaxLogRecordHeaderSize); // read logrecordheader
        header[IndexLogRecordType] = logRecord->Type;
        int index = IndexLogRecordType + 1;
        index += Code::PutVarint(header, index, logRecord->Key.size());
        index += Code::PutVarint(header, index, logRecord->Value.size());

        int size = index + logRecord->Key.size() + logRecord->Value.size();
        if(size > 40)
            throw "here is!";
        std::vector<byte> encode(size); // read logrecord based on header's keysize and valuesize
        

        std::copy(header.begin(), header.begin() + index, encode.begin());
        std::copy(logRecord->Key.begin(), logRecord->Key.end(), encode.begin() + index);
        std::copy(logRecord->Value.begin(), logRecord->Value.end(), encode.begin() + index + logRecord->Key.size());

        uint32 crc = crc32(CRC_DEFAULT, encode.data() + IndexLogRecordType, encode.size() - 4);
        std::memcpy(&encode[0], &crc, sizeof(crc));

        return encode;
    }

    static std::unique_ptr<LogRecordHeader> DecodeLogRecord(std::vector<byte> &buf)
    {
        int n = buf.size();
        if (n <= IndexLogRecordType)
            return nullptr;

        // std::unique_ptr<LogRecordHeader> header(buf[4])
        uint32 crc = *reinterpret_cast<const uint32_t*>(&buf[0]);
        int index = IndexLogRecordType + 1;

        int length = 0;
        int keySize = GetVarint(buf, index, &length);
        index += length;

        int valueSize = GetVarint(buf, index, &length);
        index += length;

        auto header = std::make_unique<LogRecordHeader>(crc, static_cast<LogRecordType>(buf[IndexLogRecordType]), keySize, valueSize, index);

        return std::move(header);
    }

    static int PutVarint(std::vector<byte> &buf, int index, int val)
    {
        int cnt = 0;
        while (val >= 0x80)
        {
            buf[index + cnt] = (0x80 | (val & 0x7f));
            val >>= 7;
            cnt++;
        }
        buf[index + cnt] = byte(val);
        return cnt + 1;
    }

    static int GetVarint(std::vector<byte> &buf, int index, int* length)
    {
        int val = 0;
        int shift = 0;
        int cnt = 0;
        while (true)
        {
            uint8_t byte = buf[index++];
            val |= (int (byte & 0x7F) << shift);
            shift += 7;
            cnt++;
            if ((byte & 0x80) == 0)
                break;
        }
        *length = cnt;
        return val;
    }
};