add_test( Batch_test.WriteBatch /home/ace/kv/build/Batch_test [==[--gtest_filter=Batch_test.WriteBatch]==] --gtest_also_run_disabled_tests)
set_tests_properties( Batch_test.WriteBatch PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( Batch_test.SeqNoBatch /home/ace/kv/build/Batch_test [==[--gtest_filter=Batch_test.SeqNoBatch]==] --gtest_also_run_disabled_tests)
set_tests_properties( Batch_test.SeqNoBatch PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( Batch_test_TESTS Batch_test.WriteBatch Batch_test.SeqNoBatch)
