cmake_minimum_required(VERSION 3.10)
project(caching-tester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Release flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Executables
add_executable(stride_access_ratio stride_access_ratio.cpp)
add_executable(line_size line_size.cpp)
