if(EXISTS "/home/ace/kv/build/main[1]_tests.cmake")
  include("/home/ace/kv/build/main[1]_tests.cmake")
else()
  add_test(main_NOT_BUILT main_NOT_BUILT)
endif()
