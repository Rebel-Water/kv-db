#include "redis/metadata.hpp"
#include "redis/types.hpp"

std::vector<byte> metadata::encode()
{
    int size = maxMetaDataSize;
    if (dataType == RedisDataType::List)
        size += extraListMetaSize;
    auto buf = std::vector<byte>(size);
    buf[0] = static_cast<byte>(dataType);
    int index = 1;
    index += Code::PutVarint(buf, index, expire);
    index += Code::PutVarint(buf, index, version);
    index += Code::PutVarint(buf, index, size);

    if (dataType == RedisDataType::List)
    {
        index += Code::PutVarint(buf, index, head);
        index += Code::PutVarint(buf, index, tail);
    }
    buf.resize(index); // modify
    return std::move(buf);
}

metadata metadata::decode(const std::vector<byte> &buf)
{
    auto dataType = static_cast<RedisDataType>(buf[0]);
    int index = 1;
    int n = 0;
    int expire = Code::GetVarint(buf, index, &n);
    index += n;
    int version = Code::GetVarint(buf, index, &n);
    index += n;
    int size = Code::GetVarint(buf, index, &n);
    index += n;
    uint64 head = 0, tail = 0;
    if (dataType == RedisDataType::List)
    {
        head = Code::GetVarint(buf, index, &n);
        index += n;
        tail = Code::GetVarint(buf, index, &n);
    }
    return {dataType, expire, version, size, head, tail};
}