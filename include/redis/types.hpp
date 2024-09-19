#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <chrono>
#include "Type.hpp"
#include "metadata.hpp"

class DB;
struct Options;
enum class RedisDataType
{
    String,
    Hash,
    Set,
    List,
    ZSet,
    NIL
};

class RedisDataStructure
{
public:
    RedisDataStructure(const Options &);

    bool ZAdd(const std::vector<byte>& key, double score, const std::vector<byte>& member);
    double ZScore(const std::vector<byte>& key, const std::vector<byte>& member);



    uint32 LPush(const std::vector<byte>& key, const std::vector<byte>& element);
    uint32 RPush(const std::vector<byte>& key, const std::vector<byte>& element);

    std::vector<byte> LPop(const std::vector<byte>& key);
    std::vector<byte> RPop(const std::vector<byte>& key);

    void Set(const std::vector<byte> &key,
             std::chrono::nanoseconds ttl,
             const std::vector<byte> &value);
    bool HSet(const std::vector<byte> &key, const std::vector<byte> &field, const std::vector<byte> &value);
    std::vector<byte> HGet(const std::vector<byte> &key, const std::vector<byte> &field);
    bool HDel(const std::vector<byte> &key, const std::vector<byte> &field);

    std::vector<byte> Get(const std::vector<byte> &key);
    void Del(const std::vector<byte> &key);
    RedisDataType Type(const std::vector<byte> &key);

private:
    uint32 pushInner(const std::vector<byte> &key, const std::vector<byte> &element, bool isLeft);
    std::vector<byte> popInner(const std::vector<byte> &key, bool isLeft);
    metadata findMetaData(const std::vector<byte> &key, RedisDataType datatype);
    std::unique_ptr<DB> db;
};