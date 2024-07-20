add_test( FileIOTest.OpenFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.OpenFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.OpenFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.WriteFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.WriteFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.WriteFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.ReadFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.ReadFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.ReadFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.SizeFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.SizeFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.SizeFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.SyncFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.SyncFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.SyncFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.CloseFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.CloseFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.CloseFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( FileIOTest.DestroyFile /home/ace/kv/build/FileIO_test [==[--gtest_filter=FileIOTest.DestroyFile]==] --gtest_also_run_disabled_tests)
set_tests_properties( FileIOTest.DestroyFile PROPERTIES WORKING_DIRECTORY /home/ace/kv/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( FileIO_test_TESTS FileIOTest.OpenFile FileIOTest.WriteFile FileIOTest.ReadFile FileIOTest.SizeFile FileIOTest.SyncFile FileIOTest.CloseFile FileIOTest.DestroyFile)
