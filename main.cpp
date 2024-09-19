#include <chrono>
#include <iostream>
#include "DB.hpp"
#include "Options.hpp"
#include "Type.hpp"
#include "Util.hpp"
#include "lib/random.hpp"
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "redis/metadata.hpp"
#include "redis/types.hpp"

int main() {

        Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);

    auto ok = redis.ZAdd(Util::ToByteVector("key1"), 113, Util::ToByteVector("value1"));
    std::cout<<ok<<" ";
    ok = redis.ZAdd(Util::ToByteVector("key1"), 333, Util::ToByteVector("value1"));
    std::cout<<ok<<" ";
    ok = redis.ZAdd(Util::ToByteVector("key1"), 98, Util::ToByteVector("value2"));
    std::cout<<ok<<" ";
    // auto score = redis.ZScore(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
    // EXPECT_EQ(score, 333);
    // score = redis.ZScore(Util::ToByteVector("key1"), Util::ToByteVector("value2"));
    // EXPECT_EQ(score, 98);

       std::filesystem::remove_all(opt.DirPath);
}

