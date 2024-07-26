#include "BTree.hpp"
#include "LogRecord.hpp"
#include "Util.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(BTreeTest, BTree_Put) {
    BTree tree;
    std::vector<byte> buf;
    LogRecordPos pos1(1, 100);
    auto res = tree.Put(buf, pos1);
    EXPECT_EQ(res, true);

    buf.push_back('a');
    LogRecordPos pos2(1, 2);
    bool flag = tree.Put(buf, pos2);
    EXPECT_EQ(flag, true);

    auto ret = tree.Get(buf);
    EXPECT_EQ(ret->Fid, 1);
    EXPECT_EQ(ret->Offset, 2);

    flag = tree.Delete(buf);
    EXPECT_EQ(flag, true);

    buf.push_back('b');
    buf.push_back('c');
    flag = tree.Delete(buf);
    EXPECT_EQ(flag, false);
}

TEST(BTreeTest, BTree_Get) {
    BTree tree;
    std::vector<byte> buf;
    buf.push_back('a');
    LogRecordPos pos1(1, 2);
    bool flag = tree.Put(buf, pos1);
    EXPECT_EQ(flag, true);

    LogRecordPos pos2(4, 3);
    flag = tree.Put(buf, pos2);
    auto ret = tree.Get(buf);
    EXPECT_EQ(ret->Fid, 4);
    EXPECT_EQ(ret->Offset, 3);

    flag = tree.Delete(buf);
    EXPECT_EQ(flag, true);
}

TEST(BTreeTest, BTree_Delete) {
    BTree tree;
    std::vector<byte> buf;
    buf.push_back('b');
    buf.push_back('c');
    bool flag = tree.Delete(buf);
    EXPECT_EQ(flag, false);
}

TEST(BTreeTest, BTree_Iterator) {
    BTree tree;
    auto iter = tree.Iter(false);
    EXPECT_EQ(false, iter->Valid());
    LogRecordPos pos(1, 2);
    std::string str("a");
    std::vector<byte> key(str.data(), str.data() + str.size());
    tree.Put(key, pos);
    iter = tree.Iter(false);
    EXPECT_EQ(true, iter->Valid());
    iter->Next();
    EXPECT_EQ(false, iter->Valid());
    tree.Put(ToByteVector("b"), pos);
    tree.Put(ToByteVector("c"), pos);
    tree.Put(ToByteVector("d"), pos);
    tree.Put(ToByteVector("e"), pos);
    iter = tree.Iter(false);
    int i = 0;
    std::string temp("abcde");
    for(iter->Rewind(); iter->Valid(); iter->Next()) {
        EXPECT_EQ(temp[i], iter->Key()[0]);
        EXPECT_EQ(true, iter->Valid());
        //std::cout << "This is an info message from GTEST_LOG_ i is: " << i << std::endl;
        i++; 
    }
    
    iter = tree.Iter(true);
    for(iter->Rewind(); iter->Valid(); iter->Next()) {
        i--;
        EXPECT_EQ(temp[i], iter->Key()[0]);
        EXPECT_EQ(true, iter->Valid());
    }
    
    iter = tree.Iter(false);
    i = 0;
    for(iter->Seek(ToByteVector("c")); iter->Valid(); iter->Next()) {
        EXPECT_EQ(temp[i + 'c' - 'a'], iter->Key()[0]);
        EXPECT_EQ(true, iter->Valid());
        i++;
    }

    iter = tree.Iter(false);
    for(iter->Seek(ToByteVector("c")); iter->Valid(); iter->Next()) {
        i--;
        EXPECT_EQ(temp[i - ('c' - 'a')], iter->Key()[0]);
        EXPECT_EQ(true, iter->Valid());
        GTEST_LOG_(INFO) << "This is an info message from GTEST_LOG_ i is: " << i << std::endl;
    }
}

// int main(int argc, char **argv) {
//   testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }
