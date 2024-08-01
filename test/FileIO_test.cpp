#include "FileIO.hpp"
#include <vector>
#include <gtest/gtest.h>
#include <exception>

TEST(FileIOTest, OpenFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}

TEST(FileIOTest, WriteFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
        std::vector<byte> buf; 
        std::string str = "hello, my kv-db";
        for(auto& ch : str)
            buf.push_back(ch);
        int n = file.Write(buf);
        EXPECT_EQ(n, buf.size());
        buf.clear();
        buf.push_back('1');
        buf.push_back('2');
        buf.push_back('3');
        n = file.Write(buf);
        EXPECT_EQ(n, buf.size());
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, ReadFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
        int size = 5, offset = 10;
        std::vector<byte> buf(size);
        int n = file.Read(buf, offset);
        EXPECT_EQ(n, size);
        std::string str;
        for(int i = 0; i < n; i++)
            str += buf[i];
        EXPECT_STREQ(str.c_str(), "kv-db");
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, SizeFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
        int size = file.Size();
        EXPECT_EQ(size, 18);
    }catch(...) {
        EXPECT_NE(1, 1);
    }
}

TEST(FileIOTest, SyncFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
        file.Sync();
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_EQ(1, 1);
}

TEST(FileIOTest, CloseFile) {
    try {
        FileIO file("/home/ace/kv/a.txt");
        file.Close();
        file.Size();
        EXPECT_EQ(1, 1);
    }catch(...) {
        EXPECT_EQ(1, 0);
    }
}

TEST(FileIOTest, DestroyFile) {
    EXPECT_EQ(0, std::remove("/home/ace/kv/a.txt"));
}

