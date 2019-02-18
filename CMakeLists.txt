# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.8)
project(libobi)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## OPTIONS
option(LIBOBI_EXAMPLES "build examples" OFF)
option(LIBOBI_TESTS "build tests" OFF)
option(LIBOBI_WARNINGS "enable warnings" ON)

message("${LIBOBI_TESTS}")

## general setup and includes
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (TestBigEndian)
TEST_BIG_ENDIAN(obi_is_big_endian)

include(obi_cmake_utils)
obi_setup()

## define header only lib
add_library(obi INTERFACE)
target_include_directories(obi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(obi INTERFACE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:LIBOBI_WARNINGS>>:${obi_stone-warnings}>
)
target_compile_features(obi INTERFACE cxx_std_17)
target_compile_definitions(obi INTERFACE
    $<IF:$<BOOL:${obi_is_big_endian}>,OBI_BIG_ENDIAN,OBI_LITTLE_ENDIAN>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_GCC}>:OBI_GCC>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_CLANG}>:OBI_CLANG>
)

## define shared lib
add_library(obi_shared SHARED
    "src/logging.cpp"
)
target_include_directories(obi_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(obi_shared PUBLIC cxx_std_17)
target_compile_options(obi_shared PRIVATE ${obi_stone-warnings})
target_compile_definitions(obi_shared PUBLIC OBI_LINK_TO_LIB=ON)
target_compile_definitions(obi_shared PUBLIC
    $<IF:$<BOOL:${obi_is_big_endian}>,OBI_BIG_ENDIAN,OBI_LITTLE_ENDIAN>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_GCC}>:OBI_GCC>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_CLANG}>:OBI_CLANG>
)

## testing
if(LIBOBI_TESTS)
    message("libobi tests enabled")
    include(CTest)
    obi_add_test_subdirectory("google" tests)
else()
    message("libobi tests disabled")
endif()

## add projects using this lib
if(LIBOBI_EXAMPLES)
    message("libobi examples enabled")
    add_subdirectory(examples)
else()
    message("libobi examples disabled")
endif()

## installation
if(COMMAND obi_install)
    obi_install(obi "include/obi")
endif()
