cmake_minimum_required(VERSION 3.14)
project(my_project)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  target_link_libraries(${test_name} GTest::gtest_main)
  gtest_discover_tests(${test_name})
endfunction()

# Add tests
add_gtest(BTree_test src/BTree.cpp BTree_test.cpp)
add_gtest(FileIO_test src/FileIO.cpp FileIO_test.cpp)
add_gtest(DataFile_test src/BTree.cpp src/FileIO.cpp src/DB.cpp DataFile_test.cpp)


# cmake_minimum_required(VERSION 3.14)
# project(my_project)

# # GoogleTest requires at least C++14
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(include)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

# FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_executable(
#   BTree_test
#   src/BTree.cpp
#   BTree_test.cpp
# )

# add_executable(
#   FileIO_test
#   src/FileIO.cpp
#   FileIO_test.cpp
# )

# add_executable(
#   DataFile_test
#   src/FileIO.cpp
#   DataFile_test.cpp
# )

# target_link_libraries(
#   BTree_test
#   GTest::gtest_main
# )

# target_link_libraries(
#   FileIO_test
#   GTest::gtest_main
# )

# target_link_libraries(
#   DataFile_test
#   GTest::gtest_main
# )

# include(GoogleTest)
# gtest_discover_tests(BTree_test)
# gtest_discover_tests(FileIO_test)
# gtest_discover_tests(DataFile_test)