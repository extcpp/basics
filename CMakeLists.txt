# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(libobi)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLib")

option(LIBOBI_EXAMPLES ON)

## set os specific flags
include(obi_util)
obi_settings()

## define lib
add_library(libobi INTERFACE)
target_include_directories(libobi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/include")

## define tests
if(TEST)
    # build tests
    # to come
    # run tests
    enable_testing()
    add_Test(NAME "foo" COMMAND do_test arg --arg WORKING_DIRECTORY tests)
endif()

## add projects using this lib
if(LIBOBI_EXAMPLES)
    add_subdirectory(Examples)
endif()
