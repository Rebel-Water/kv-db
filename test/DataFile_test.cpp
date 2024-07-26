#include "DataFile.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(DataFile_test, DataFile) {
    try {
        auto datafile0 = std::make_unique<DataFile>("/home/ace/kv/", 0);
        auto datafile1 = std::make_unique<DataFile>("/home/ace/kv/", 0);
        auto datafile2 = std::make_unique<DataFile>("/home/ace/kv/", 1234);
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}

TEST(DataFile_test, WriteAndSyncFile) {
    try {
        auto datafile0 = std::make_unique<DataFile>("/home/ace/kv/", 0);
        std::vector<byte> buf{'a', 'b', 'c', 'd', 'e'};
        datafile0->Write(buf);
        datafile0->Sync();
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}

TEST(DataFile_test, CloseFile) {
    try {
        auto datafile0 = std::make_unique<DataFile>("/home/ace/kv/", 0);
        datafile0->Close();
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}

TEST(DataFile_test, DestroyFile) {
    try {
        std::remove("/home/ace/kv/000000000.data");
        std::remove("/home/ace/kv/000001234.data");
    }catch(...) {
        EXPECT_NE(1, 1);
    }
    EXPECT_NE(1, 0);
}



