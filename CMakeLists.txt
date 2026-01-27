cmake_minimum_required(VERSION 3.10)
project(caching-tester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executables
add_executable(stride_access_ratio stride_access_ratio.cpp)

# Platform-specific compiler flags
if(MSVC)
    # Windows (MSVC)
    target_compile_options(stride_access_ratio PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /Ot /DNDEBUG>
    )
else()
    # Unix (GCC/Clang)
    target_compile_options(stride_access_ratio PRIVATE
        -Wall -Wextra
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )
endif()
