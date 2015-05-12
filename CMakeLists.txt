# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(libobi)

## set os specific flags
include(./compiler-warnings.cmake)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${stone-warnings}")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    # replace with CMAKE_CXX_STANDARD when a newer cmake becomes
    # available.
    # set(CMAKE_CXX_STANDARD 14)
    # ##set_property(TARGET libobi PROPERTY CXX_STANDARD 14) <-- this would be nice
endif()

## define lib
add_library(libobi INTERFACE)
target_include_directories(libobi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

## define tests
if(TEST)
    # build tests
    # to come
    # run tests
    enable_testing()
    add_Test(NAME "foo" COMMAND do_test arg --arg WORKING_DIRECTORY tests)
endif()

## add projects using this lib
add_subdirectory(Examples)
