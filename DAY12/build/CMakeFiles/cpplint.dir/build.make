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
CMAKE_SOURCE_DIR = /home/hu/WebSever/30DAYS/DAY12

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hu/WebSever/30DAYS/DAY12/build

# Utility rule file for cpplint.

# Include any custom commands dependencies for this target.
include CMakeFiles/cpplint.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cpplint.dir/progress.make

CMakeFiles/cpplint:
	echo '/home/hu/WebSever/30DAYS/DAY12/src/Acceptor.cpp /home/hu/WebSever/30DAYS/DAY12/src/Buffer.cpp /home/hu/WebSever/30DAYS/DAY12/src/Channel.cpp /home/hu/WebSever/30DAYS/DAY12/src/Connection.cpp /home/hu/WebSever/30DAYS/DAY12/src/EventLoop.cpp /home/hu/WebSever/30DAYS/DAY12/src/InetAddress.cpp /home/hu/WebSever/30DAYS/DAY12/src/Server.cpp /home/hu/WebSever/30DAYS/DAY12/src/Socket.cpp /home/hu/WebSever/30DAYS/DAY12/src/ThreadPool.cpp /home/hu/WebSever/30DAYS/DAY12/src/epoll.cpp /home/hu/WebSever/30DAYS/DAY12/src/include/Acceptor.h /home/hu/WebSever/30DAYS/DAY12/src/include/Buffer.h /home/hu/WebSever/30DAYS/DAY12/src/include/Channel.h /home/hu/WebSever/30DAYS/DAY12/src/include/Connection.h /home/hu/WebSever/30DAYS/DAY12/src/include/EventLoop.h /home/hu/WebSever/30DAYS/DAY12/src/include/InetAddress.h /home/hu/WebSever/30DAYS/DAY12/src/include/Server.h /home/hu/WebSever/30DAYS/DAY12/src/include/Socket.h /home/hu/WebSever/30DAYS/DAY12/src/include/ThreadPool.h /home/hu/WebSever/30DAYS/DAY12/src/include/epoll.h /home/hu/WebSever/30DAYS/DAY12/src/include/util.h /home/hu/WebSever/30DAYS/DAY12/src/util.cpp /home/hu/WebSever/30DAYS/DAY12/test/ThreadPoolTest.cpp /home/hu/WebSever/30DAYS/DAY12/test/multipile_client.cpp /home/hu/WebSever/30DAYS/DAY12/test/server.cpp /home/hu/WebSever/30DAYS/DAY12/test/single_client.cpp /home/hu/WebSever/30DAYS/DAY12/test/test.cpp' | xargs -n12 -P8 /home/hu/.local/bin/cpplint --verbose=2 --quiet --linelength=120 --filter=-legal/copyright,-build/include_subdir,-readability/casting

cpplint: CMakeFiles/cpplint
cpplint: CMakeFiles/cpplint.dir/build.make
.PHONY : cpplint

# Rule to build all files generated by this target.
CMakeFiles/cpplint.dir/build: cpplint
.PHONY : CMakeFiles/cpplint.dir/build

CMakeFiles/cpplint.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpplint.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpplint.dir/clean

CMakeFiles/cpplint.dir/depend:
	cd /home/hu/WebSever/30DAYS/DAY12/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hu/WebSever/30DAYS/DAY12 /home/hu/WebSever/30DAYS/DAY12 /home/hu/WebSever/30DAYS/DAY12/build /home/hu/WebSever/30DAYS/DAY12/build /home/hu/WebSever/30DAYS/DAY12/build/CMakeFiles/cpplint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpplint.dir/depend

