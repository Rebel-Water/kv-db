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
    std::filesystem::remove_all("redis/");
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    auto flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field1"), Util::ToByteVector("value1"));
    std::cout<<flag<<std::endl;
    flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field1"), Util::ToByteVector("value1"));
    std::cout<<flag<<std::endl;
    flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field2"), Util::ToByteVector("value1"));
    std::cout<<flag<<std::endl;
}
