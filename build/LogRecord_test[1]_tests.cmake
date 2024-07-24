add_test( LogRecord_Test.EncodeAndDecodeLogRecord /home/ace/kv/build/LogRecord_test [==[--gtest_filter=LogRecord_Test.EncodeAndDecodeLogRecord]==] --gtest_also_run_disabled_tests)
set_tests_properties( LogRecord_Test.EncodeAndDecodeLogRecord PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( LogRecord_test_TESTS LogRecord_Test.EncodeAndDecodeLogRecord)
