#include "BTree.hpp"
#include "LogRecord.hpp"
#include "Code.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(LogRecord_Test, EncodeAndDecodeLogRecord) {
    const char* data = "name Mr.sun";
    std::vector<byte> key(data, data + 4);
    std::vector<byte> value(data + 5, data + strlen(data));
    LogRecord logrecord(key, value, LogRecordType::LogRecordNormal);
    auto res = Code::EncodeLogRecord(logrecord);
    EXPECT_GT(res.size(), 5);
    auto header = Code::DecodeLogRecord(res);
    EXPECT_EQ(header->keySize, 4);
    EXPECT_EQ(header->valueSize, 6);
    EXPECT_GT(header->headerSize, 0);
    EXPECT_EQ(header->recordType, LogRecordType::LogRecordNormal);
}
