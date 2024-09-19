#include "Batch.hpp"
#include "DB.hpp"
#include "Util.hpp"
#include "Options.hpp"
#include <gtest/gtest.h>
#include <exception>
#include <thread>
#include "redis/types.hpp"
TEST(redis_type_test, set_and_get_test)
{
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

TEST(redis_type_test, del_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    redis.Del(Util::ToByteVector("tiktok"));
    EXPECT_EQ(redis.Get(Util::ToByteVector("tiktok")).size(), 0);
    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, string_type_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    auto type = redis.Type(Util::ToByteVector("tiktok"));
    EXPECT_EQ(type, RedisDataType::String);
    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, hash_type_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    auto flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field1"), Util::ToByteVector("value1"));
    EXPECT_EQ(flag, true);
    flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field1"), Util::ToByteVector("value2"));
    EXPECT_EQ(flag, false);
    flag = redis.HSet(Util::ToByteVector("key1"), Util::ToByteVector("field2"), Util::ToByteVector("value3"));
    EXPECT_EQ(flag, true);

    auto val1 = redis.HGet(Util::ToByteVector("key1"), Util::ToByteVector("field1"));
    EXPECT_EQ(val1, Util::ToByteVector("value2"));
    auto val2 = redis.HGet(Util::ToByteVector("key1"), Util::ToByteVector("field2"));
    EXPECT_EQ(val2, Util::ToByteVector("value3"));

    flag = redis.HDel(Util::ToByteVector("key1"), Util::ToByteVector("field1"));
    EXPECT_EQ(flag, true);
    flag = redis.HDel(Util::ToByteVector("key1"), Util::ToByteVector("field2"));
    EXPECT_EQ(flag, true);
        flag = redis.HDel(Util::ToByteVector("key1"), Util::ToByteVector("field2"));
        EXPECT_EQ(flag, false);

    try
    {
        val1 = redis.HGet(Util::ToByteVector("key1"), Util::ToByteVector("field1"));
        EXPECT_NE(1, 1);
    }
    catch (const std::exception &e)
    {
        EXPECT_EQ(1, 1);
    }

    std::filesystem::remove_all(opt.DirPath);
}