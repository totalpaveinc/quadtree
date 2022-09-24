
cmake_minimum_required(VERSION 3.22.1)

project(tpquadtree VERSION 0.0.0)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "") 
    SET(CMAKE_BUILD_TYPE "Debug")
endif()

if (
    (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release") AND
    (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
)
    message(FATAL_ERROR "Invalid build type \"${CMAKE_BUILD_TYPE}\" for xcodebuild")
endif()

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tpquadtree STATIC
    include/tp/qt/QuadTree.h
    include/tp/qt/Node.h
    include/tp/qt/Point.h

    src/QuadTree.cpp
    src/Node.cpp
    src/Point.cpp
)

# Extent Dependency
if (NOT DEFINED TP_EXTENT_LIBRARY)
    add_subdirectory(third_party/extent extent)
    target_include_directories(tpquadtree
        PUBLIC third_party/extent/include
        PUBLIC ./include
    )
    target_link_libraries(tpquadtree tpgeomextent)
else()
    if (NOT DEFINED TP_EXTENT_LIB_DIR)
        message(FATAL_ERROR "TP_EXTENT_LIB_DIR is not set. Set it to the path that contains ${TP_EXTENT_LIBRARY}")
    endif()
    if (NOT DEFINED TP_EXTENT_INCLUDE_DIR)
        message(FATAL_ERROR "TP_EXTENT_INCLUDE_DIR is not set. Set it to the path that contains Extent library's include/")
    endif()

    target_include_directories(tpquadtree PUBLIC ${TP_EXTENT_INCLUDE_DIR})
    target_link_directories(tpquadtree PUBLIC ${TP_EXTENT_LIB_DIR})
    target_link_libraries(tpquadtree ${TP_EXTENT_LIBRARY})
endif()

if (${TP_QUADTREE_BUILD_TESTS})
    message(STATUS "Building QuadTree Test App")
    add_executable(qttest test/main.cpp)
    target_link_libraries(qttest tpquadtree)
    target_compile_options(qttest
        PUBLIC "-Wall"  # Suppose to allow tracing on all errors
        PUBLIC "-g"     # Suppose to add additional debug symbols, intended for debuggers like GDB
        PUBLIC "-O0"    # Do no optimizations, we will want to set this to level 2 for release
        PUBLIC "-fexceptions" # Enable exceptions and throw calls
    )
    target_link_options(qttest
        PUBLIC "-rdynamic"
    )
else()
    message(STATUS "Skipping QuadTree Test App")
endif()
