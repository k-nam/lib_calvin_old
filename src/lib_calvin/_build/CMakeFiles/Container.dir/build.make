# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build

# Include any dependencies generated for this target.
include CMakeFiles/Container.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Container.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Container.dir/flags.make

CMakeFiles/Container.dir/container/container_main.cc.o: CMakeFiles/Container.dir/flags.make
CMakeFiles/Container.dir/container/container_main.cc.o: ../container/container_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Container.dir/container/container_main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Container.dir/container/container_main.cc.o -c /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/container/container_main.cc

CMakeFiles/Container.dir/container/container_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Container.dir/container/container_main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/container/container_main.cc > CMakeFiles/Container.dir/container/container_main.cc.i

CMakeFiles/Container.dir/container/container_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Container.dir/container/container_main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/container/container_main.cc -o CMakeFiles/Container.dir/container/container_main.cc.s

CMakeFiles/Container.dir/container/container_main.cc.o.requires:

.PHONY : CMakeFiles/Container.dir/container/container_main.cc.o.requires

CMakeFiles/Container.dir/container/container_main.cc.o.provides: CMakeFiles/Container.dir/container/container_main.cc.o.requires
	$(MAKE) -f CMakeFiles/Container.dir/build.make CMakeFiles/Container.dir/container/container_main.cc.o.provides.build
.PHONY : CMakeFiles/Container.dir/container/container_main.cc.o.provides

CMakeFiles/Container.dir/container/container_main.cc.o.provides.build: CMakeFiles/Container.dir/container/container_main.cc.o


# Object files for target Container
Container_OBJECTS = \
"CMakeFiles/Container.dir/container/container_main.cc.o"

# External object files for target Container
Container_EXTERNAL_OBJECTS =

Container: CMakeFiles/Container.dir/container/container_main.cc.o
Container: CMakeFiles/Container.dir/build.make
Container: CMakeFiles/Container.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Container"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Container.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Container.dir/build: Container

.PHONY : CMakeFiles/Container.dir/build

CMakeFiles/Container.dir/requires: CMakeFiles/Container.dir/container/container_main.cc.o.requires

.PHONY : CMakeFiles/Container.dir/requires

CMakeFiles/Container.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Container.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Container.dir/clean

CMakeFiles/Container.dir/depend:
	cd /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build /home/ubuntu/src/calvincaulfield/lib_calvin/src/lib_calvin/_build/CMakeFiles/Container.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Container.dir/depend

