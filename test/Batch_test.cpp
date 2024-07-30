#include "Batch.hpp"
#include "DB.hpp"
#include "Util.hpp"
#include "Options.hpp"
#include <gtest/gtest.h>
#include <exception>

TEST(Batch_test, WriteBatch) {
    try {
        Options option;
        DB  db(option);
        WriteBatchOptions wbOption;
        auto wb = db.NewWriteBatch(wbOption);
        wb->Put(Util::ToByteVector("first"), Util::ToByteVector("1"));
        try {
            db.Get(Util::ToByteVector("first"));
        } catch(const std::exception& e) {
            EXPECT_STREQ(e.what(), "DB::Get Key Not Found");
        }
        wb->Put(Util::ToByteVector("second"), Util::ToByteVector("2"));
        wb->Delete(Util::ToByteVector("first"));
        wb->Commit();
        auto str = Util::ToString(db.Get(Util::ToByteVector("second")));
        EXPECT_STREQ(str.data(), "2");
        wb->Put(Util::ToByteVector("third"), Util::ToByteVector("3"));
        wb->Commit();
        EXPECT_EQ(wb->db->seqNo, 2);
        db.Get(Util::ToByteVector("first"));
    } catch(const std::exception& e) {
        EXPECT_STREQ(e.what(), "DB::Get Key Not Found");
    }
}

TEST(Batch_test, SeqNoBatch) {
    try {
        Options option;
        DB  db(option);
        //  WriteBatchOptions wbOption;
        //  wbOption.MaxBatchNum = 10000000;
        // auto wb = db.NewWriteBatch(wbOption);
        // for(int i = 0; i < 5000000; i++) {
        //      std::string str("test" + std::to_string(i));
        //      wb->Put(Util::ToByteVector(str), Util::ToByteVector("End"));
        // }
        // wb->Commit();
        auto keys = db.ListKey();  
        // GTEST_LOG_(INFO) << keys.size();
        EXPECT_EQ(keys.size(), 2);
        std::remove("/home/ace/kv/000000000.data");
    } catch(const std::exception& e) {
        GTEST_LOG_(INFO) << e.what();
    }
}
