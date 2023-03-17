# Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.25)
project(ext-basics VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "[extcpp] entering ext::basics")

## OPTIONS
option(EXT_DOWNLOAD "allow dependency download" OFF)

option(EXT_WRITE_VERSION "write version.hpp" OFF)
option(LIBEXT_EXAMPLES "build examples" OFF)
option(LIBEXT_TESTS "build tests" OFF)
option(LIBEXT_TESTS_NO_TIME_CRITICAL "disable time critical tests" OFF)

### configure defer
option(LIBEXT_WARNINGS "enable warnings" ON)
option(LIBEXT_CHECKED  "user assert" ON)
option(EXT_DEFER_NON_NOTHROW "allow throwing functions in defer" OFF)
option(EXT_DEFER_MOVE_ASSIGN "allow move assignment of defer object" OFF)

include(use_ext_foundation.cmake)
ext_colorize_output()

include(src_files.cmake)
include(include_files.cmake)

set(ext_basics_compile_definitions
    $<$<BOOL:${LIBEXT_TESTS_NO_TIME_CRITICAL}>:EXT_TESTS_NO_TIME_CRITICAL=1>
    $<$<BOOL:${EXT_DEFER_NON_NOTHROW}>:EXT_DEFER_ALLOW_NON_NOTHROW_INVOCABLE=1>
    $<$<BOOL:${EXT_DEFER_MOVE_ASSIGN}>:EXT_DEFER_ENABLE_MOVE_ASSIGN=1>
)

find_package(Threads REQUIRED)

### define header only lib
add_library(ext-basics INTERFACE)
target_link_libraries(ext-basics INTERFACE ext::foundation)
target_include_directories(ext-basics INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_compile_features(ext-basics INTERFACE cxx_std_20)
target_compile_options(ext-basics INTERFACE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:LIBEXT_WARNINGS>>:${ext_stone-warnings}>
)
target_compile_definitions(ext-basics INTERFACE
    ${ext_basics_compile_definitions}
    EXT_BASICS_HEADER_ONLY_LIB
)

# set up folder structure for XCode and VisualStudio
#set_target_properties (ext-basics PROPERTIES FOLDER libext) #not for interface libraries
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ext-basics-header} ${ext-basics-source})

add_library(ext::basics ALIAS ext-basics)

## testing
if(LIBEXT_TESTS)
    include(CTest)
    add_subdirectory(tests)
else()
    ext_log("ext-basics tests disabled")
endif()

## add projects using this lib
if(LIBEXT_EXAMPLES)
    ext_log("ext-basics examples enabled")
    add_subdirectory(examples)
else()
    ext_log("ext-basics examples disabled")
endif()

## installation
if(COMMAND ext_install_lib)
    ext_set_default_install_prefix("$ENV{HOME}/.local")
    set_target_properties(ext-basics PROPERTIES EXPORT_NAME basics)
    ext_install_lib(ext-basics "ext::" ExtBasics include/ext)
endif()

ext_generate_version_hpp(basics)
