cmake_minimum_required(VERSION 3.10)
project(caching-tester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Platform-specific compiler flags
if(MSVC)
    set(DEBUG_FLAGS /Od /Zi)
    set(RELEASE_FLAGS /O2 /Ot /DNDEBUG)
else()
    set(DEBUG_FLAGS -Wall -Wextra -g -O0)
    set(RELEASE_FLAGS -Wall -Wextra -O3 -DNDEBUG)
endif()

# Executables
add_executable(stride_access_ratio stride_access_ratio.cpp)
target_compile_options(stride_access_ratio PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

if(UNIX)
    add_executable(latency_vs_size latency_vs_size.cpp)
    target_compile_options(latency_vs_size PRIVATE
        $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
        $<$<CONFIG:Release>:${RELEASE_FLAGS}>
    )
endif()
