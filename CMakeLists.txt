# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.8)
project(libobi)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#message(FATAL_ERROR "## ${CMAKE_FIND_LIBRARY_SUFFIXES}")
include(obi_fm_util)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (TestBigEndian)
TEST_BIG_ENDIAN(obi_is_big_endian)

if(COMMAND obi_setup_with_test)
    obi_setup_with_test()
endif()

separate_arguments(obi_stone-warnings)

## define header only lib
add_library(obi INTERFACE)
target_compile_definitions(obi INTERFACE $<IF:$<BOOL:${obi_is_big_endian}>,OBI_BIG_ENDIAN,OBI_LITTLE_ENDIAN>)
target_include_directories(obi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(obi INTERFACE cxx_std_17)
target_compile_options(obi INTERFACE $<$<CONFIG:Debug>:${obi_stone-warnings}>)

## define shared lib
add_library(obi_shared SHARED
    "src/logging.cpp"
)
target_compile_definitions(obi_shared PUBLIC OBI_LINK_TO_LIB=ON)
target_compile_definitions(obi_shared PUBLIC $<IF:$<BOOL:${obi_is_big_endian}>,OBI_BIG_ENDIAN,OBI_LITTLE_ENDIAN>)
target_include_directories(obi_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(obi_shared PUBLIC cxx_std_17)
target_compile_options(obi_shared PRIVATE $<$<CONFIG:Debug>:${obi_stone-warnings}>)

## testing
obi_testing("google")

## add projects using this lib
option(LIBOBI_EXAMPLES ON)
if(LIBOBI_EXAMPLES)
    add_subdirectory(examples)
endif()

if(COMMAND obi_install)
    obi_install(obi "include/obi")
endif()

# Tooling is now able to find the file. This file was not copied
# reliable because the file did not always exist when the CMakeLists.txt
# was parsed.

#if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
#   EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
#        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
#   )
#endif()
