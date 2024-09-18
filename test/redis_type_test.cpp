#include "Batch.hpp"
#include "DB.hpp"
#include "Util.hpp"
#include "Options.hpp"
#include <gtest/gtest.h>
#include <exception>
#include <thread>
#include "redis/types.hpp"
TEST(redis_type_test, set_and_get_test) {
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    auto val = redis.Get(Util::ToByteVector("tiktok"));
    EXPECT_EQ(val, Util::ToByteVector("china"));
    redis.Set(Util::ToByteVector("google"), std::chrono::seconds(5), Util::ToByteVector("usa"));
    val = redis.Get(Util::ToByteVector("google"));
    EXPECT_EQ(val, Util::ToByteVector("usa"));
    std::this_thread::sleep_for(std::chrono::seconds(6));
    val = redis.Get(Util::ToByteVector("google"));
    EXPECT_EQ(val.size(), 0);
    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, del_test) {
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    redis.Del(Util::ToByteVector("tiktok"));
    EXPECT_EQ(redis.Get(Util::ToByteVector("tiktok")).size(), 0);
    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, type_test) {
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    auto type = redis.Type(Util::ToByteVector("tiktok"));
    EXPECT_EQ(type, RedisDataType::String);
    std::filesystem::remove_all(opt.DirPath);
}