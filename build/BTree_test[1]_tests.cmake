add_test( BTreeTest.BTree_Put /home/ace/kv/build/BTree_test [==[--gtest_filter=BTreeTest.BTree_Put]==] --gtest_also_run_disabled_tests)
set_tests_properties( BTreeTest.BTree_Put PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( BTreeTest.BTree_Get /home/ace/kv/build/BTree_test [==[--gtest_filter=BTreeTest.BTree_Get]==] --gtest_also_run_disabled_tests)
set_tests_properties( BTreeTest.BTree_Get PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( BTreeTest.BTree_Delete /home/ace/kv/build/BTree_test [==[--gtest_filter=BTreeTest.BTree_Delete]==] --gtest_also_run_disabled_tests)
set_tests_properties( BTreeTest.BTree_Delete PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( BTree_test_TESTS BTreeTest.BTree_Put BTreeTest.BTree_Get BTreeTest.BTree_Delete)
