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
        db.Put(Util::ToByteVector("key1"), Util::ToByteVector("value1"));
        auto res1 = db.Get(Util::ToByteVector("key1"));
        EXPECT_EQ(res1, Util::ToByteVector("value1"));
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
        auto res3 = db.Get(Util::ToByteVector("key_c"));;
        EXPECT_STREQ("value_c", Util::ToString(res3).c_str());
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
        db.Put(Util::ToByteVector("a"), Util::ToByteVector("a"));
        db.Put(Util::ToByteVector("b"), Util::ToByteVector("b"));
        db.Put(Util::ToByteVector("bc"), Util::ToByteVector("bc"));
        db.Put(Util::ToByteVector("bd"), Util::ToByteVector("bd"));
        db.Put(Util::ToByteVector("e"), Util::ToByteVector("e"));
        IteratorOptions iteropt(Util::ToByteVector("b"));
        auto iter = db.NewIterator(iteropt);
        // std::string strs[5] =
        // GTEST_LOG_(INFO) << "Test!!!" << std::endl;
        for (iter->Rewind(); iter->Valid(); iter->Next())
        {
            GTEST_LOG_(INFO) << Util::ToString(iter->Value()) << std::endl;
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
        std::string strs[6] = {"a", "b", "bc", "bd", "e", "f"};
        EXPECT_EQ(db.Get(Util::ToByteVector("a")), Util::ToByteVector("a"));
        db.Put(Util::ToByteVector("f"), Util::ToByteVector("f"));
        int i = 0;
        for(auto& key : keys) {
            EXPECT_STREQ(Util::ToString(key).data(), strs[i++].data());
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
        std::remove("/home/ace/kv/data/000000000.txt");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

TEST(DB_TEST, DBMerge) {
    try
    {
        Options option;
        DB db(option);
        db.Merge(); // just merge for fun
        db.Put(Util::ToByteVector("a"), Util::ToByteVector("a"));
        db.Put(Util::ToByteVector("b"), Util::ToByteVector("b"));
        db.Put(Util::ToByteVector("bc"), Util::ToByteVector("bc"));
        db.Put(Util::ToByteVector("bd"), Util::ToByteVector("bd"));
        db.Put(Util::ToByteVector("e"), Util::ToByteVector("e"));
        db.Delete(Util::ToByteVector("a"));
        db.Merge();
        db.Sync();
        db.Close();
        DB db2(option);
        EXPECT_EQ(db2.Get(Util::ToByteVector("b")), Util::ToByteVector("b"));
        db2.Get(Util::ToByteVector("a"));
    }
    catch (const std::exception& e)
    {
        GTEST_LOG_(INFO) << e.what() << std::endl;
        EXPECT_STREQ(e.what(), "DB::Get Key Not Found");
    }
}

TEST(DB_TEST, DBMerge2) {
    try
    {
        Options option;
        DB db(option);
        auto str = Util::ToString(db.Get(Util::ToByteVector("bc")));
        EXPECT_STREQ(str.data(), "bc");
        std::remove("/home/ace/kv/data/000000000.txt");
        std::remove("/home/ace/kv/data/000000001.txt");
    }
    catch (const std::exception& e)
    {
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_TEST, DBFileLock) {
    try
    {
        Options option;
        DB db1(option);
        DB db2(option);
    }
    catch (const std::exception& e)
    {
        EXPECT_STREQ(e.what(), "DB::Open File Lock Occupied");
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_TEST, DBStatement) {
    try {
        Options option;
        DB db(option);
        for(int i = 0; i < 100; i++)
            db.Put(Util::ToByteVector("123123" + std::to_string(i)), Util::ToByteVector("1231231"));
        for(int i = 0; i < 100; i++)
            db.Delete(Util::ToByteVector("123123" + std::to_string(i)));
        db.Sync();
        auto stat = db.Statement();
        EXPECT_EQ(stat.dataFileNum, 2);
        EXPECT_GE(stat.diskSize, 100);
        EXPECT_EQ(stat.keyNum, 0);
        EXPECT_GE(stat.reclaimableSize, 100);
        GTEST_LOG_(INFO) << "reclaimableSize: " << stat.reclaimableSize << std::endl;
    } catch(const std::exception& e) {
        GTEST_LOG_(INFO) << e.what() << std::endl;
    }
}

TEST(DB_TEST, DBBackUp) {
    try {
        Options option;
        DB db(option);
        db.BackUp("/home/ace/kv/backup");
    } catch(const std::exception& e) {
        GTEST_LOG_(INFO) << e.what() << std::endl;
        EXPECT_NE(1, 1);
    }
    std::remove("/home/ace/kv/data/000000000.txt");
    std::filesystem::remove_all("/home/ace/kv/backup");
    std::filesystem::remove_all("/home/ace/kv/data");
}


