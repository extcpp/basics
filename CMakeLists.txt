# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.2.3)
project(libobi)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
include(obi_fm_util)
obi_setup_with_test()

## define header only lib
add_library(obi INTERFACE)
target_include_directories(obi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
separate_arguments(obi_stone-warnings)

## define shared lib
add_library(obi_shared SHARED
    "src/math.cpp"
)
target_compile_definitions(obi_shared PUBLIC OBI_LINK_TO_LIB=ON)
target_include_directories(obi_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(obi_shared PRIVATE $<$<CONFIG:Debug>:${obi_stone-warnings}>)

## testing
obi_testing("google")

## add projects using this lib
option(LIBOBI_EXAMPLES ON)
if(LIBOBI_EXAMPLES)
    add_subdirectory(examples)
endif()

obi_install(obi "include/obi")

if( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()
