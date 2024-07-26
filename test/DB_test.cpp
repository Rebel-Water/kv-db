#include "BTree.hpp"
#include "LogRecord.hpp"
#include "Util.hpp"
#include "Code.hpp"
#include "Options.hpp"
#include "DB.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(DB_Test, DBPutAndGet)
{
    try
    {
        Options option;
        DB db(option);
        const char *k1 = "key_a";
        const char *k2 = "key_b";
        const char *k3 = "key_c";
        const char *v1 = "value_a";
        const char *v2 = "value_b";
        const char *v3 = "value_c";
        std::vector<byte> key1(k1, k1 + strlen(k1)), value1(v1, v1 + strlen(v1));
        std::vector<byte> key2(k2, k2 + strlen(k2)), value2(v2, v2 + strlen(v2));
        std::vector<byte> key3(k3, k3 + strlen(k3)), value3(v3, v3 + strlen(v3));
        db.Put(key1, value1);
        auto res1 = db.Get(key1);
        EXPECT_EQ(res1, value1);
        db.Put(key2, value2);
        auto res2 = db.Get(key2);
        EXPECT_EQ(res2, value2);
        db.Put(key1, value2);
        auto res3 = db.Get(key1);
        EXPECT_EQ(res3, value2);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

TEST(DB_Test, DBIndexLoadFromDataFile)
{
    try
    {
        Options option;
        DB db(option);
        const char *k1 = "key_a";
        const char *k2 = "key_b";
        const char *v1 = "value_a";
        const char *v2 = "value_b";
        std::vector<byte> key1(k1, k1 + strlen(k1)), value1(v1, v1 + strlen(v1));
        std::vector<byte> key2(k2, k2 + strlen(k2)), value2(v2, v2 + strlen(v2));
        auto res1 = db.Get(key1);
        EXPECT_EQ(res1, value2);
        auto res2 = db.Get(key2);
        EXPECT_EQ(res2, value2);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

TEST(DB_Test, DBDelete)
{
    try
    {
        Options option;
        DB db(option);

        const char *k1 = "key_a";
        const char *k2 = "key_b";
        const char *v1 = "value_a";
        const char *v2 = "value_b";
        std::vector<byte> key1(k1, k1 + strlen(k1)), value1(v1, v1 + strlen(v1));
        std::vector<byte> key2(k2, k2 + strlen(k2)), value2(v2, v2 + strlen(v2));
        db.Delete(key1);
        db.Delete(key2);
        // db.Put(key1, value1);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

TEST(DB_Test, DBIterator)
{
    try
    {
        Options option;
        DB db(option);
        db.Put(ToByteVector("a"), ToByteVector("a"));
        db.Put(ToByteVector("b"), ToByteVector("b"));
        db.Put(ToByteVector("bc"), ToByteVector("bc"));
        db.Put(ToByteVector("bd"), ToByteVector("bd"));
        db.Put(ToByteVector("e"), ToByteVector("e"));
        IteratorOptions iteropt(ToByteVector("b"));
        auto iter = db.NewIterator(iteropt);
        // std::string strs[5] =
        // GTEST_LOG_(INFO) << "Test!!!" << std::endl;
        for (iter->Rewind(); iter->Valid(); iter->Next())
        {
            GTEST_LOG_(INFO) << ToString(iter->Value()) << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_Test, DBFold)
{
    try
    {
        Options option;
        DB db(option);
        db.Fold([](const std::vector<byte>& , const std::vector<byte>&) -> bool{
            return true;
        });
    }
    catch (const std::exception& e)
    {
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_Test, DBListKey)
{
    try
    {
        Options option;
        DB db(option);
        auto keys = db.ListKey();
        std::string strs[5] = {"a", "b", "bc", "bd", "e"};
        int i = 0;
        for(auto& key : keys) {
            EXPECT_STREQ(ToString(key).data(), strs[i++].data());
        }
    }
    catch (const std::exception& e)
    {
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_Test, DestroyFiles)
{
    try
    {
        std::remove("/home/ace/kv/000000000.data");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}