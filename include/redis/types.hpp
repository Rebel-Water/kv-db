#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <chrono>
#include "Type.hpp"
#include "metadata.hpp"

class DB;
struct Options;
enum class RedisDataType {
    String,
    Hash,
    Set,
    List,
    ZSet,
    NIL
};


class RedisDataStructure {
    public:
    RedisDataStructure(const Options&);
    void Set(const std::vector<byte>& key, 
                                   std::chrono::nanoseconds ttl, 
                                   const std::vector<byte>& value);
    bool HSet(const std::vector<byte>& key, const std::vector<byte>& field, const std::vector<byte>& value);
    std::vector<byte> HGet(const std::vector<byte>& key, const std::vector<byte>& field);
    bool HDel(const std::vector<byte>& key, const std::vector<byte>& field);

    std::vector<byte> Get(const std::vector<byte>& key);
    void Del(const std::vector<byte>& key);
    RedisDataType Type(const std::vector<byte>& key);

    private:
    metadata findMetaData(const std::vector<byte>& key, RedisDataType datatype);
    std::unique_ptr<DB> db;

};