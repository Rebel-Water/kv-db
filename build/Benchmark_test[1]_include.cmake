if(EXISTS "/home/ace/kv/build/Benchmark_test[1]_tests.cmake")
  include("/home/ace/kv/build/Benchmark_test[1]_tests.cmake")
else()
  add_test(Benchmark_test_NOT_BUILT Benchmark_test_NOT_BUILT)
endif()
