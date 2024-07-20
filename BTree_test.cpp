#include "BTree.hpp"
#include "LogRecord.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(BTreeTest, BTree_Put) {
    BTree tree;
    std::vector<byte> buf;
    auto res = tree.Put(buf, std::make_unique<LogRecordPos>(1, 100));
    EXPECT_EQ(res, true);

    buf.push_back('a');
    bool flag = tree.Put(buf, std::make_unique<LogRecordPos>(1, 2));
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
    bool flag = tree.Put(buf, std::make_unique<LogRecordPos>(1, 2));
    EXPECT_EQ(flag, true);

    auto ret = tree.Get(buf);
    EXPECT_EQ(ret->Fid, 1);
    EXPECT_EQ(ret->Offset, 2);

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

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
