#include "Code.hpp"
#include <optional>
#include <memory>

std::vector<byte> Code::EncodeLogRecord(LogRecord& logRecord)
{
    std::vector<byte> header(MaxLogRecordHeaderSize); // read logrecordheader
    header[IndexLogRecordType] = static_cast<byte>(logRecord.Type);
    int index = IndexLogRecordType + 1;
    index += Code::PutVarint(header, index, logRecord.Key.size());
    index += Code::PutVarint(header, index, logRecord.Value.size());

    int size = index + logRecord.Key.size() + logRecord.Value.size();

    std::vector<byte> encode(size); // read logrecord based on header's keysize and valuesize

    std::copy(header.begin(), header.begin() + index, encode.begin());
    std::copy(logRecord.Key.begin(), logRecord.Key.end(), encode.begin() + index);
    std::copy(logRecord.Value.begin(), logRecord.Value.end(), encode.begin() + index + logRecord.Key.size());

    uint32 crc = crc32(CRC_DEFAULT, encode.data() + IndexLogRecordType, encode.size() - 4);
    std::memcpy(&encode[0], &crc, sizeof(crc));

    return std::move(encode);
}

std::optional<LogRecordHeader> Code::DecodeLogRecord(const std::vector<byte> &buf)
{
    int n = buf.size();
    if (n <= IndexLogRecordType)
        return std::nullopt;

    uint32 crc = *reinterpret_cast<const uint32_t *>(&buf[0]);
    int index = IndexLogRecordType + 1;

    int length = 0;
    int keySize = GetVarint(buf, index, &length);
    index += length;

    int valueSize = GetVarint(buf, index, &length);
    index += length;

    auto header = LogRecordHeader(crc, static_cast<LogRecordType>(buf[IndexLogRecordType]), keySize, valueSize, index);
    return std::make_optional(header);
}

std::vector<byte> Code::EncodeLogRecordPos(LogRecordPos& pos)
{
    std::vector<byte> buf(MaxVarintLen32 + MaxVarintLen64);
    int index = 0;
    index += Code::PutVarint(buf, index, int64(pos.Fid));
    index += Code::PutVarint(buf, index, int64(pos.Offset));
    buf.resize(index);
    return std::move(buf);
}

LogRecordPos Code::DecodeLogRecordPos(const std::vector<byte> &buf)
{
    int index = 0;
    int length = 0;
    int fileId = GetVarint(buf, index, &length);
    index += length;
    int offset = GetVarint(buf, index, &length);
    return std::move(LogRecordPos(fileId, offset));
}

int Code::PutVarint(std::vector<byte> &buf, int index, int val)
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

int Code::GetVarint(const std::vector<byte> &buf, int index, int *length)
{
    int val = 0;
    int shift = 0;
    int cnt = 0;
    while (true)
    {
        uint8_t byte = buf[index++];
        val |= (int(byte & 0x7F) << shift);
        shift += 7;
        cnt++;
        if ((byte & 0x80) == 0)
            break;
    }
    *length = cnt;
    return val;
}

int Code::PutUint64(std::vector<byte> &buf, int index, int val)
{
    for(int cnt = 0; cnt < 8; cnt++)
    {
        buf[index + cnt] = (val & 0xff);
        val >>= 8;
    }
    return sizeof(uint64);
}

int Code::GetUint64(const std::vector<byte> &buf, int index)
{
    int val = 0;
    for(int cnt = 0; cnt < 8; cnt++)
    {
        val <<= 8;
        val |= buf[index + cnt];
    }
    return val;
}
