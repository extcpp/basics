# Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.8)
project(libobi)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## OPTIONS
option(LIBOBI_EXAMPLES "build examples" OFF)
option(LIBOBI_TESTS "build tests" OFF)
option(LIBOBI_WARNINGS "enable warnings" ON)

## general setup and includes
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # XCode / VS folders

include (TestBigEndian)
TEST_BIG_ENDIAN(obi_is_big_endian)

include(obi_cmake_utils)
obi_setup()

# verbose windows linking
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /VERBOSE:LIB")

if(UNIX)
    execute_process(COMMAND getconf LEVEL1_DCACHE_LINESIZE OUTPUT_VARIABLE obi_l1_cache_line_size)
    string(STRIP ${obi_l1_cache_line_size} obi_l1_cache_line_size)
    obi_log("cache line size: '${obi_l1_cache_line_size}'")
    set(OBI_OUTDIR "")
elseif(MSVC)
    set(obi_l1_cache_line_size FALSE)
    set(OBI_OUTDIR "${PROJECT_BINARY_DIR}/build")
    obi_log("binary ouput dir: ${OBI_OUTDIR}")

    #TODO - move settings below into corresponding targets
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OBI_OUTDIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OBI_OUTDIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OBI_OUTDIR}")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else()
    set(obi_l1_cache_line_size FALSE)
endif()

# required by folder structure for XCode and VisualStudio (includes)
# sources are always required
include(src_files.cmake)
include(include_files.cmake)

### library setup
set(obi_common_private_compile_definitions
    $<IF:$<BOOL:${obi_is_big_endian}>,OBI_BIG_ENDIAN,OBI_LITTLE_ENDIAN>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_GCC}>:OBI_GCC>
    $<$<BOOL:${OBI_CXX_COMPILER_IS_CLANG}>:OBI_CLANG>
    $<$<BOOL:${obi_l1_cache_line_size}>:OBI_KNOWN_L1_CACHE_LINE_SIZE=${obi_l1_cache_line_size}>
    OBI_IN_LIB=1
)

### define header only lib
add_library(obi INTERFACE)
target_include_directories(obi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(obi INTERFACE cxx_std_17)
target_compile_options(obi INTERFACE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:LIBOBI_WARNINGS>>:${obi_stone-warnings}>
)
target_compile_definitions(obi INTERFACE
    ${obi_common_private_compile_definitions}
    OBI_HEADER_ONLY_LIB
)

### define shared lib
add_library(obi_shared SHARED ${libobi_source} ${libobi_header})
target_include_directories(obi_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(obi_shared PUBLIC cxx_std_17)
target_compile_options(obi_shared PRIVATE ${obi_stone-warnings})
target_compile_definitions(obi_shared PRIVATE ${obi_common_private_compile_definitions})
target_compile_definitions(obi_shared PUBLIC OBI_SHARED_LIB)

# set up folder structure for XCode and VisualStudio
set_target_properties (obi_shared PROPERTIES FOLDER libobi)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${libobi_header} ${libobi_source})

## testing
if(LIBOBI_TESTS)
    obi_log("libobi tests enabled")
    include(CTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    obi_add_test_subdirectory("google" tests)
else()
    obi_log("libobi tests disabled")
endif()

## add projects using this lib
if(LIBOBI_EXAMPLES)
    obi_log("libobi examples enabled")
    add_subdirectory(examples)
else()
    obi_log("libobi examples disabled")
endif()

## installation
if(COMMAND obi_install)
    obi_install(obi "include/obi")
endif()
