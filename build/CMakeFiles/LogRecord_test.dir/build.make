# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ace/kv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ace/kv/build

# Include any dependencies generated for this target.
include CMakeFiles/LogRecord_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/LogRecord_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/LogRecord_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LogRecord_test.dir/flags.make

CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o: ../include/lib/skiplist.cpp
CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o -MF CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o.d -o CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o -c /home/ace/kv/include/lib/skiplist.cpp

CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/include/lib/skiplist.cpp > CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.i

CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/include/lib/skiplist.cpp -o CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.s

CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o: ../src/DataFile.cpp
CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o -c /home/ace/kv/src/DataFile.cpp

CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/DataFile.cpp > CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.i

CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/DataFile.cpp -o CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.s

CMakeFiles/LogRecord_test.dir/src/Code.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/Code.cpp.o: ../src/Code.cpp
CMakeFiles/LogRecord_test.dir/src/Code.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/LogRecord_test.dir/src/Code.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/Code.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/Code.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/Code.cpp.o -c /home/ace/kv/src/Code.cpp

CMakeFiles/LogRecord_test.dir/src/Code.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/Code.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/Code.cpp > CMakeFiles/LogRecord_test.dir/src/Code.cpp.i

CMakeFiles/LogRecord_test.dir/src/Code.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/Code.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/Code.cpp -o CMakeFiles/LogRecord_test.dir/src/Code.cpp.s

CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o: ../src/Batch.cpp
CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o -c /home/ace/kv/src/Batch.cpp

CMakeFiles/LogRecord_test.dir/src/Batch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/Batch.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/Batch.cpp > CMakeFiles/LogRecord_test.dir/src/Batch.cpp.i

CMakeFiles/LogRecord_test.dir/src/Batch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/Batch.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/Batch.cpp -o CMakeFiles/LogRecord_test.dir/src/Batch.cpp.s

CMakeFiles/LogRecord_test.dir/src/DB.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/DB.cpp.o: ../src/DB.cpp
CMakeFiles/LogRecord_test.dir/src/DB.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/LogRecord_test.dir/src/DB.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/DB.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/DB.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/DB.cpp.o -c /home/ace/kv/src/DB.cpp

CMakeFiles/LogRecord_test.dir/src/DB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/DB.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/DB.cpp > CMakeFiles/LogRecord_test.dir/src/DB.cpp.i

CMakeFiles/LogRecord_test.dir/src/DB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/DB.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/DB.cpp -o CMakeFiles/LogRecord_test.dir/src/DB.cpp.s

CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o: ../src/Mmap.cpp
CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o -c /home/ace/kv/src/Mmap.cpp

CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/Mmap.cpp > CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.i

CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/Mmap.cpp -o CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.s

CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o: ../src/FileIO.cpp
CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o -c /home/ace/kv/src/FileIO.cpp

CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/FileIO.cpp > CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.i

CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/FileIO.cpp -o CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.s

CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o: ../src/BTree.cpp
CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o -c /home/ace/kv/src/BTree.cpp

CMakeFiles/LogRecord_test.dir/src/BTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/BTree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/BTree.cpp > CMakeFiles/LogRecord_test.dir/src/BTree.cpp.i

CMakeFiles/LogRecord_test.dir/src/BTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/BTree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/BTree.cpp -o CMakeFiles/LogRecord_test.dir/src/BTree.cpp.s

CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o: ../test/LogRecord_test.cpp
CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o -MF CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o.d -o CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o -c /home/ace/kv/test/LogRecord_test.cpp

CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/test/LogRecord_test.cpp > CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.i

CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/test/LogRecord_test.cpp -o CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.s

CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o: CMakeFiles/LogRecord_test.dir/flags.make
CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o: ../src/SkipList.cpp
CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o: CMakeFiles/LogRecord_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o -MF CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o.d -o CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o -c /home/ace/kv/src/SkipList.cpp

CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/SkipList.cpp > CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.i

CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/SkipList.cpp -o CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.s

# Object files for target LogRecord_test
LogRecord_test_OBJECTS = \
"CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/Code.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/DB.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o" \
"CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o" \
"CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o"

# External object files for target LogRecord_test
LogRecord_test_EXTERNAL_OBJECTS =

LogRecord_test: CMakeFiles/LogRecord_test.dir/include/lib/skiplist.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/DataFile.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/Code.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/Batch.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/DB.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/Mmap.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/FileIO.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/BTree.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/test/LogRecord_test.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/src/SkipList.cpp.o
LogRecord_test: CMakeFiles/LogRecord_test.dir/build.make
LogRecord_test: lib/libgtest_main.a
LogRecord_test: /usr/lib/x86_64-linux-gnu/libz.so
LogRecord_test: lib/libgtest.a
LogRecord_test: CMakeFiles/LogRecord_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable LogRecord_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LogRecord_test.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=LogRecord_test -D TEST_EXECUTABLE=/home/ace/kv/build/LogRecord_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/ace/kv/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=LogRecord_test_TESTS -D CTEST_FILE=/home/ace/kv/build/LogRecord_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.22/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/LogRecord_test.dir/build: LogRecord_test
.PHONY : CMakeFiles/LogRecord_test.dir/build

CMakeFiles/LogRecord_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LogRecord_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LogRecord_test.dir/clean

CMakeFiles/LogRecord_test.dir/depend:
	cd /home/ace/kv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ace/kv /home/ace/kv /home/ace/kv/build /home/ace/kv/build /home/ace/kv/build/CMakeFiles/LogRecord_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LogRecord_test.dir/depend

