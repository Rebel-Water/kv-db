cmake_minimum_required(VERSION 3.14)
project(my_project)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug")


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

find_package(ZLIB REQUIRED)
find_package(benchmark REQUIRED)

# Include project headers
include_directories(include)

# set(BENCHMARK_USE_BUNDLED_GTEST OFF)
# set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
# Fetch and set up GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  SOURCE_DIR /home/ace/kv/googletest-03597a01ee50ed33e9dfd640b249b4be3799d395
)
FetchContent_MakeAvailable(googletest)

# # Enable testing
enable_testing()

# # Include GoogleTest module
include(GoogleTest)

# # Function to simplify adding tests
function(add_gtest test_name)
  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name} GTest::gtest_main ZLIB::ZLIB)
  gtest_discover_tests(${test_name})
endfunction()


# Add tests
add_gtest(SkipList_test include/lib/skiplist.cpp src/FileIO.cpp src/Mmap.cpp src/DataFile.cpp src/Code.cpp src/SkipList.cpp test/SkipList_test.cpp)
add_gtest(BTree_test src/FileIO.cpp src/Mmap.cpp src/DataFile.cpp src/Code.cpp src/BTree.cpp test/BTree_test.cpp)
add_gtest(FileIO_test include/lib/skiplist.cpp src/FileIO.cpp src/Mmap.cpp src/DataFile.cpp src/Code.cpp src/FileIO.cpp test/FileIO_test.cpp src/SkipList.cpp)
add_gtest(Mmap_test include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Mmap.cpp src/FileIO.cpp test/Mmap_test.cpp src/SkipList.cpp)
add_gtest(DataFile_test include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/BTree.cpp src/Mmap.cpp src/FileIO.cpp src/DB.cpp test/DataFile_test.cpp src/SkipList.cpp)
add_gtest(LogRecord_test include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/DB.cpp src/Mmap.cpp src/FileIO.cpp src/BTree.cpp test/LogRecord_test.cpp src/SkipList.cpp)
add_gtest(DB_test include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/Iterator.cpp src/DB.cpp src/Mmap.cpp src/FileIO.cpp src/BTree.cpp test/DB_test.cpp src/SkipList.cpp)
add_gtest(Batch_test include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/DB.cpp src/Mmap.cpp src/BTree.cpp src/FileIO.cpp test/Batch_test.cpp src/SkipList.cpp)
add_gtest(redis_type_test src/redis/metadata.cpp src/redis/types.cpp test/redis_type_test.cpp include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/Iterator.cpp src/DB.cpp src/Mmap.cpp src/FileIO.cpp src/BTree.cpp test/DB_test.cpp src/SkipList.cpp)

# add_executable(bench_mark test/Benchmark_test.cpp src/DB.cpp include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/Iterator.cpp  src/Mmap.cpp src/FileIO.cpp src/BTree.cpp src/SkipList.cpp src/FileIO.cpp)
# target_link_libraries(bench_mark benchmark::benchmark ZLIB::ZLIB)

# add_executable(main main.cpp src/redis/metadata.cpp src/redis/types.cpp include/lib/skiplist.cpp src/DataFile.cpp src/Code.cpp src/Batch.cpp src/Iterator.cpp src/DB.cpp src/Mmap.cpp src/FileIO.cpp src/BTree.cpp src/SkipList.cpp)
# target_link_libraries(main ZLIB::ZLIB)

