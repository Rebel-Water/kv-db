#pragma once
#include "Code.hpp"

class RedisDataStructure;
enum class RedisDataType;

struct hashInternalKey {
    std::vector<byte> key;
    int64 version;
    std::vector<byte> field;
    public:
    hashInternalKey(const std::vector<byte>& key, int64 version, const std::vector<byte>& field)
        : key(key), version(version), field(field) {}

    std::vector<byte> encode() {
        auto buf = std::vector<byte>(key.size() + field.size() + 8);
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin() + index);
        index += key.size();
        index += Code::PutUint64(buf, index, version);
        std::copy(field.begin(), field.end(), buf.begin() + index);
        return std::move(buf);
    }
};


struct metadata
{
    static const int maxMetaDataSize = MaxVarintLen64 * 2 + MaxVarintLen32 + 1;
    static const int extraListMetaSize = MaxVarintLen64 * 2;
    static const int initialListMark = UINT32_MAX / 2;
    metadata() = default;
    metadata(RedisDataType dataType,
             int64 expire,
             int64 version,
             int size,
             uint64 head,
             uint64 tail) : dataType(dataType), expire(expire), version(version), size(size), head(head), tail(tail)
    {
    }

    std::vector<byte> encode();

    static metadata decode(const std::vector<byte> &buf);
    
    friend class RedisDataStructure;

    RedisDataType dataType;
    int64 expire;
    int64 version;
    int size;
    uint64 head; // for list
    uint64 tail; // for list
};