# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/grigorijpenkin/Documents/pr_a/algo/ex_12

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ex_12.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ex_12.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ex_12.dir/flags.make

CMakeFiles/ex_12.dir/main.cpp.o: CMakeFiles/ex_12.dir/flags.make
CMakeFiles/ex_12.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ex_12.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex_12.dir/main.cpp.o -c /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/main.cpp

CMakeFiles/ex_12.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex_12.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/main.cpp > CMakeFiles/ex_12.dir/main.cpp.i

CMakeFiles/ex_12.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex_12.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/main.cpp -o CMakeFiles/ex_12.dir/main.cpp.s

# Object files for target ex_12
ex_12_OBJECTS = \
"CMakeFiles/ex_12.dir/main.cpp.o"

# External object files for target ex_12
ex_12_EXTERNAL_OBJECTS =

ex_12: CMakeFiles/ex_12.dir/main.cpp.o
ex_12: CMakeFiles/ex_12.dir/build.make
ex_12: CMakeFiles/ex_12.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ex_12"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex_12.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ex_12.dir/build: ex_12

.PHONY : CMakeFiles/ex_12.dir/build

CMakeFiles/ex_12.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ex_12.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ex_12.dir/clean

CMakeFiles/ex_12.dir/depend:
	cd /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/grigorijpenkin/Documents/pr_a/algo/ex_12 /Users/grigorijpenkin/Documents/pr_a/algo/ex_12 /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug /Users/grigorijpenkin/Documents/pr_a/algo/ex_12/cmake-build-debug/CMakeFiles/ex_12.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ex_12.dir/depend

