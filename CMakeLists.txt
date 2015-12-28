# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(libobi)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(obi_fm_util)
obi_setup_with_test()

## define lib
add_library(libobi INTERFACE)
target_include_directories(libobi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/include")

obi_testing("google")

## add projects using this lib
option(LIBOBI_EXAMPLES ON)
if(LIBOBI_EXAMPLES)
    add_subdirectory(examples)
endif()

obi_install(libobi "src/include/obi")
