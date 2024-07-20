#include "FileIO.hpp"
#include <vector>
#include <gtest/gtest.h>
#include <exception>

TEST(FileIOTest, OpenFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}

TEST(FileIOTest, WriteFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
        std::string buf = "hello, my kv-db";
        int n = file.Write(buf.c_str(), buf.size());
        EXPECT_EQ(n, buf.size());
        buf = "123";
        n = file.Write(buf.c_str(), buf.size());
        EXPECT_EQ(n, buf.size());
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, ReadFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
        int size = 5, offset = 10;
        char buf[10] = {0};
        int n = file.Read(buf ,size, offset);
        EXPECT_EQ(n, size);
        EXPECT_STREQ(buf, "kv-db");
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, SizeFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
        int size = file.Size();
        EXPECT_EQ(size, 18);
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, SyncFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
        file.Sync();
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_EQ(1, 1);
}

TEST(FileIOTest, CloseFile) {
    try {
        FileIO file("/home/ace/kv/a.data");
        file.Close();
        file.Size();
        EXPECT_EQ(1, 1);
    }catch(...) {
        EXPECT_EQ(1, 1);
    }
}

TEST(FileIOTest, DestroyFile) {
    EXPECT_EQ(0, std::remove("/home/ace/kv/a.data"));
}

