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
include CMakeFiles/DataFile_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DataFile_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DataFile_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DataFile_test.dir/flags.make

CMakeFiles/DataFile_test.dir/src/BTree.cpp.o: CMakeFiles/DataFile_test.dir/flags.make
CMakeFiles/DataFile_test.dir/src/BTree.cpp.o: ../src/BTree.cpp
CMakeFiles/DataFile_test.dir/src/BTree.cpp.o: CMakeFiles/DataFile_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DataFile_test.dir/src/BTree.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataFile_test.dir/src/BTree.cpp.o -MF CMakeFiles/DataFile_test.dir/src/BTree.cpp.o.d -o CMakeFiles/DataFile_test.dir/src/BTree.cpp.o -c /home/ace/kv/src/BTree.cpp

CMakeFiles/DataFile_test.dir/src/BTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataFile_test.dir/src/BTree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/BTree.cpp > CMakeFiles/DataFile_test.dir/src/BTree.cpp.i

CMakeFiles/DataFile_test.dir/src/BTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataFile_test.dir/src/BTree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/BTree.cpp -o CMakeFiles/DataFile_test.dir/src/BTree.cpp.s

CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o: CMakeFiles/DataFile_test.dir/flags.make
CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o: ../src/FileIO.cpp
CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o: CMakeFiles/DataFile_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o -MF CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o.d -o CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o -c /home/ace/kv/src/FileIO.cpp

CMakeFiles/DataFile_test.dir/src/FileIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataFile_test.dir/src/FileIO.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/FileIO.cpp > CMakeFiles/DataFile_test.dir/src/FileIO.cpp.i

CMakeFiles/DataFile_test.dir/src/FileIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataFile_test.dir/src/FileIO.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/FileIO.cpp -o CMakeFiles/DataFile_test.dir/src/FileIO.cpp.s

CMakeFiles/DataFile_test.dir/src/DB.cpp.o: CMakeFiles/DataFile_test.dir/flags.make
CMakeFiles/DataFile_test.dir/src/DB.cpp.o: ../src/DB.cpp
CMakeFiles/DataFile_test.dir/src/DB.cpp.o: CMakeFiles/DataFile_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/DataFile_test.dir/src/DB.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataFile_test.dir/src/DB.cpp.o -MF CMakeFiles/DataFile_test.dir/src/DB.cpp.o.d -o CMakeFiles/DataFile_test.dir/src/DB.cpp.o -c /home/ace/kv/src/DB.cpp

CMakeFiles/DataFile_test.dir/src/DB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataFile_test.dir/src/DB.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/src/DB.cpp > CMakeFiles/DataFile_test.dir/src/DB.cpp.i

CMakeFiles/DataFile_test.dir/src/DB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataFile_test.dir/src/DB.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/src/DB.cpp -o CMakeFiles/DataFile_test.dir/src/DB.cpp.s

CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o: CMakeFiles/DataFile_test.dir/flags.make
CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o: ../DataFile_test.cpp
CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o: CMakeFiles/DataFile_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o -MF CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o.d -o CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o -c /home/ace/kv/DataFile_test.cpp

CMakeFiles/DataFile_test.dir/DataFile_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataFile_test.dir/DataFile_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ace/kv/DataFile_test.cpp > CMakeFiles/DataFile_test.dir/DataFile_test.cpp.i

CMakeFiles/DataFile_test.dir/DataFile_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataFile_test.dir/DataFile_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ace/kv/DataFile_test.cpp -o CMakeFiles/DataFile_test.dir/DataFile_test.cpp.s

# Object files for target DataFile_test
DataFile_test_OBJECTS = \
"CMakeFiles/DataFile_test.dir/src/BTree.cpp.o" \
"CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o" \
"CMakeFiles/DataFile_test.dir/src/DB.cpp.o" \
"CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o"

# External object files for target DataFile_test
DataFile_test_EXTERNAL_OBJECTS =

DataFile_test: CMakeFiles/DataFile_test.dir/src/BTree.cpp.o
DataFile_test: CMakeFiles/DataFile_test.dir/src/FileIO.cpp.o
DataFile_test: CMakeFiles/DataFile_test.dir/src/DB.cpp.o
DataFile_test: CMakeFiles/DataFile_test.dir/DataFile_test.cpp.o
DataFile_test: CMakeFiles/DataFile_test.dir/build.make
DataFile_test: lib/libgtest_main.a
DataFile_test: /usr/lib/x86_64-linux-gnu/libz.so
DataFile_test: lib/libgtest.a
DataFile_test: CMakeFiles/DataFile_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ace/kv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable DataFile_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DataFile_test.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=DataFile_test -D TEST_EXECUTABLE=/home/ace/kv/build/DataFile_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/ace/kv/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=DataFile_test_TESTS -D CTEST_FILE=/home/ace/kv/build/DataFile_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.22/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/DataFile_test.dir/build: DataFile_test
.PHONY : CMakeFiles/DataFile_test.dir/build

CMakeFiles/DataFile_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DataFile_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DataFile_test.dir/clean

CMakeFiles/DataFile_test.dir/depend:
	cd /home/ace/kv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ace/kv /home/ace/kv /home/ace/kv/build /home/ace/kv/build /home/ace/kv/build/CMakeFiles/DataFile_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DataFile_test.dir/depend

