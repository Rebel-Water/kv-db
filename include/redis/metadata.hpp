#pragma once
#include "Code.hpp"

class RedisDataStructure;
enum class RedisDataType;

struct setInternalKey {
    std::vector<byte> key;
    int64 version;
    std::vector<byte> member;
        setInternalKey(const std::vector<byte>& key, int64 version, const std::vector<byte>& member)
        : key(key), version(version), member(member) {}

    std::vector<byte> encode() {
        auto buf = std::vector<byte>(key.size() + member.size() + sizeof(version) + sizeof(version));
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin());
        index += key.size();
        index += Code::PutUint64(buf, index, version);
        std::copy(member.begin(), member.end(), buf.begin() + index);
        index += member.size();
        Code::PutUint64(buf, index, member.size());
        return std::move(buf);
    }
};

struct zsetInternalKey {
    std::vector<byte> key;
    int64 version;
    std::vector<byte> member;
    double score;
    public:
    zsetInternalKey(const std::vector<byte>& key, int64 version, const std::vector<byte>& member, double score)
        : key(key), version(version), member(member), score(score) {}

    std::vector<byte> encodeWithMember() {
        auto buf = std::vector<byte>(key.size() + member.size() + sizeof(int64));
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin());
        index += key.size();
        Code::PutUint64(buf, index, version);
        index += 8;

        std::copy(member.begin(), member.end(), buf.begin() + index);

        return buf;
    }
    std::vector<byte> encodeWithScore() {
        auto scores = Code::Float64ToBytes(score);
        auto buf = std::vector<byte>(key.size() + member.size() + sizeof(int64) + sizeof(double));
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin());
        index += key.size();
        Code::PutUint64(buf, index, version);
        index += 8;
        std::copy(scores.begin(), scores.end(), buf.begin() + index);
        index += scores.size();
        
        Code::PutUint64(buf, index, member.size());

        return buf;
    }

};

struct hashInternalKey {
    std::vector<byte> key;
    int64 version;
    std::vector<byte> field;
    public:
    hashInternalKey(const std::vector<byte>& key, int64 version, const std::vector<byte>& field)
        : key(key), version(version), field(field) {}

    std::vector<byte> encode() {
        auto buf = std::vector<byte>(key.size() + field.size() + sizeof(version));
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin());
        index += key.size();
        index += Code::PutUint64(buf, index, version);
        std::copy(field.begin(), field.end(), buf.begin() + index);
        return std::move(buf);
    }
};

struct listInternalKey {
    std::vector<byte> key;
    int64 version;
    uint64 index;
    listInternalKey(const std::vector<byte>& key, int64 version, uint64 index)
        : key(key), version(version), index(index) {}

    std::vector<byte> encode() {
        auto buf = std::vector<byte>(key.size() + sizeof(version) + sizeof(index));
        int index = 0;
        std::copy(key.begin(), key.end(), buf.begin());
        index += key.size(); 
        index += Code::PutUint64(buf, index, version);
        index += Code::PutUint64(buf, index, this->index);

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