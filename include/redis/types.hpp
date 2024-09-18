#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <chrono>
#include "Type.hpp"

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
    std::vector<byte> Get(const std::vector<byte>& key);
    void Del(const std::vector<byte>& key);
    RedisDataType Type(const std::vector<byte>& key);
    private:
    std::unique_ptr<DB> db;


};