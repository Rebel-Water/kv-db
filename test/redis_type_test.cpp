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
    try
    {
        val = redis.Get(Util::ToByteVector("google"));
        EXPECT_NE(1, 1);
    }
    catch (...)
    {
        EXPECT_EQ(1, 1);
    }

    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, del_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set(Util::ToByteVector("tiktok"), std::chrono::seconds(0), Util::ToByteVector("china"));
    redis.Del(Util::ToByteVector("tiktok"));
    try
    {
        redis.Get(Util::ToByteVector("tiktok"));
        EXPECT_NE(1, 1);
    }
    catch (...)
    {
        EXPECT_EQ(1, 1);
    }
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
    }
    catch (const std::exception &e)
    {
        EXPECT_EQ(1, 1);
    }

    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, list_type_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    auto res = redis.LPush(Util::ToByteVector("key1"), Util::ToByteVector("val1"));
    EXPECT_EQ(res, 1);
    res = redis.LPush(Util::ToByteVector("key1"), Util::ToByteVector("val1"));
    EXPECT_EQ(res, 2);
    res = redis.LPush(Util::ToByteVector("key1"), Util::ToByteVector("val2"));
    EXPECT_EQ(res, 3);

    auto val = redis.RPop(Util::ToByteVector("key1"));
    EXPECT_EQ(val, Util::ToByteVector("val1"));

    val = redis.LPop(Util::ToByteVector("key1"));
    EXPECT_EQ(val, Util::ToByteVector("val2"));

    val = redis.RPop(Util::ToByteVector("key1"));
    EXPECT_EQ(val, Util::ToByteVector("val1"));

    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, zset_type_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);

    auto ok = redis.ZAdd(Util::ToByteVector("key1"), 113, Util::ToByteVector("value1"));
    EXPECT_EQ(ok, true);
    ok = redis.ZAdd(Util::ToByteVector("key1"), 333, Util::ToByteVector("value1"));
    EXPECT_EQ(ok, false);
    ok = redis.ZAdd(Util::ToByteVector("key1"), 98, Util::ToByteVector("value2"));
    EXPECT_EQ(ok, true);
    auto score = redis.ZScore(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
    EXPECT_EQ(score, 333);
    score = redis.ZScore(Util::ToByteVector("key1"), Util::ToByteVector("value2"));
    EXPECT_EQ(score, 98);

    std::filesystem::remove_all(opt.DirPath);
}

TEST(redis_type_test, set_type_test)
{
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);

    auto ok = redis.SAdd(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
    EXPECT_EQ(ok, true);
    ok = redis.SAdd(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
    EXPECT_EQ(ok, false);
    ok = redis.SAdd(Util::ToByteVector("key1"), Util::ToByteVector("value2"));
    EXPECT_EQ(ok, true);

    ok = redis.SIsMember(Util::ToByteVector("key2"), Util::ToByteVector("value1"));
    EXPECT_EQ(ok, false);
    ok = redis.SIsMember(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
    EXPECT_EQ(ok, true);
    ok = redis.SIsMember(Util::ToByteVector("key1"), Util::ToByteVector("value2"));
    EXPECT_EQ(ok, true);
    ok = redis.SIsMember(Util::ToByteVector("key1"), Util::ToByteVector("value not exist"));
    EXPECT_EQ(ok, false);

    ok = redis.SRem(Util::ToByteVector("key2"), Util::ToByteVector("value1"));
    EXPECT_EQ(ok, false);
    ok = redis.SRem(Util::ToByteVector("key1"), Util::ToByteVector("value2"));
    EXPECT_EQ(ok, true);
    
    std::filesystem::remove_all(opt.DirPath);
}