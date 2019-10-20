# Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.8)
project(libext)
message(STATUS "extINFO -- entering libext")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## OPTIONS
option(LIBEXT_EXAMPLES "build examples" OFF)
option(LIBEXT_TESTS "build tests" OFF)
option(LIBEXT_WARNINGS "enable warnings" ON)
option(LIBEXT_CHECKED "user assert" ON)

## general setup and includes
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # XCode / VS folders

include (TestBigEndian)
TEST_BIG_ENDIAN(ext_is_big_endian)

set(LIBEXT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include(ext_cmake_utils)
ext_setup()

# verbose windows linking
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /VERBOSE:LIB")

if(LINUX)
    execute_process(COMMAND getconf LEVEL1_DCACHE_LINESIZE OUTPUT_VARIABLE ext_l1_cache_line_size)
    string(STRIP ${ext_l1_cache_line_size} ext_l1_cache_line_size)
    ext_log("cache line size: '${ext_l1_cache_line_size}'")
    set(EXT_OUTDIR "")
elseif(MSVC)
    set(ext_l1_cache_line_size FALSE)
    ext_log("cache line size: provided by os")
    set(EXT_OUTDIR "${PROJECT_BINARY_DIR}/build")
    ext_log("binary ouput dir: ${EXT_OUTDIR}")

    #TODO - move settings below into corresponding targets
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${EXT_OUTDIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${EXT_OUTDIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXT_OUTDIR}")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
elseif(APPLE)
    set(ext_l1_cache_line_size "64")
    ext_log("cache line size: '${ext_l1_cache_line_size}'")
else()
    set(ext_l1_cache_line_size FALSE)
endif()

# include build dir to find version.hpp
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# required by folder structure for XCode and VisualStudio (includes)
# sources are always required
include(src_files.cmake)
include(include_files.cmake)

### library setup
set(ext_common_compile_definitions
    $<IF:$<BOOL:${ext_is_big_endian}>,EXT_BIG_ENDIAN,EXT_LITTLE_ENDIAN>
    $<$<BOOL:${EXT_CXX_COMPILER_IS_GCC}>:EXT_GCC>
    $<$<BOOL:${EXT_CXX_COMPILER_IS_CLANG}>:EXT_CLANG>
    $<$<BOOL:${ext_l1_cache_line_size}>:EXT_KNOWN_L1_CACHE_LINE_SIZE=${ext_l1_cache_line_size}>
    $<$<BOOL:${LIBEXT_CHECKED}>:EXT_CHECKED=1>
)


find_package(Threads REQUIRED)
### detect if allocators sub-lib is present
set(EXTALLOCATORS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/allocators/include)
if(EXISTS ${EXTALLOCATORS_PATH})
    ext_log("found ext allocators")
else()
    #unset path if the include directory does not exists -- not checked out
    unset(EXTALLOCATORS_PATH)
endif()

### define header only lib
add_library(ext INTERFACE)
target_include_directories(ext INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${EXTALLOCATORS_PATH}"
)
target_compile_features(ext INTERFACE cxx_std_17)
target_compile_options(ext INTERFACE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:LIBEXT_WARNINGS>>:${ext_stone-warnings}>
)
target_compile_definitions(ext INTERFACE
    ${ext_common_compile_definitions}
    EXT_HEADER_ONLY_LIB
)

### define shared lib
add_library(ext_shared SHARED ${libext_source} ${libext_header})
target_include_directories(ext_shared PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${EXTALLOCATORS_PATH}"
)
target_compile_features(ext_shared PUBLIC cxx_std_17)
target_compile_options(ext_shared PRIVATE ${ext_stone-warnings})
target_compile_definitions(ext_shared PUBLIC ${ext_common_compile_definitions})
target_compile_definitions(ext_shared PUBLIC EXT_SHARED_LIB)
target_compile_definitions(ext_shared PRIVATE EXT_IN_LIB=1)
target_link_libraries(ext_shared Threads::Threads)


# set up folder structure for XCode and VisualStudio
set_target_properties (ext_shared PROPERTIES FOLDER libext)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${libext_header} ${libext_source})

## testing
if(LIBEXT_TESTS)
    ext_log("libext tests enabled")
    include(CTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    ext_add_test_subdirectory("google" tests)
else()
    ext_log("libext tests disabled")
endif()

## add projects using this lib
if(LIBEXT_EXAMPLES)
    ext_log("libext examples enabled")
    add_subdirectory(examples)
else()
    ext_log("libext examples disabled")
endif()

## installation
if(COMMAND ext_install)
    ext_install(ext "include/ext")
endif()

add_custom_target(
    update_version ALL
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND}
                -D "EXT_GIT_VERSION_OUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/ext_version.hpp"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/script_git_version.cmake"
)
