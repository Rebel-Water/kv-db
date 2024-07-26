cmake_minimum_required(VERSION 3.14)
project(my_project)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)

# Include project headers
include_directories(include)

# Fetch and set up GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Include GoogleTest module
include(GoogleTest)

# Function to simplify adding tests
function(add_gtest test_name)
  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name} GTest::gtest_main ZLIB::ZLIB)
  gtest_discover_tests(${test_name})
endfunction()

# add_executable(main src/DB.cpp src/FileIO.cpp src/BTree.cpp main.cpp)
# target_link_libraries(main ZLIB::ZLIB)

# Add tests
add_gtest(BTree_test src/BTree.cpp test/BTree_test.cpp)
add_gtest(FileIO_test src/FileIO.cpp test/FileIO_test.cpp)
add_gtest(DataFile_test src/BTree.cpp src/FileIO.cpp src/DB.cpp test/DataFile_test.cpp)
add_gtest(LogRecord_test src/DB.cpp src/FileIO.cpp src/BTree.cpp test/LogRecord_test.cpp)
add_gtest(DB_test src/Iterator.cpp src/DB.cpp src/FileIO.cpp src/BTree.cpp test/DB_test.cpp)
