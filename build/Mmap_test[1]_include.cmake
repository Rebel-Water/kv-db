if(EXISTS "/home/ace/kv/build/Mmap_test[1]_tests.cmake")
  include("/home/ace/kv/build/Mmap_test[1]_tests.cmake")
else()
  add_test(Mmap_test_NOT_BUILT Mmap_test_NOT_BUILT)
endif()
