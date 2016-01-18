# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(obi_examples)

## required libs
find_package(Threads)

## examples

set(examples
    functional
    function_cache
    if
    load_library
    meta
    scoped_timer
    static_lambda
    type_to_string
    container_helper
    vector
    concurrency
    scope_guard
    show
    tuple
    if_all_none
)

foreach(ex IN LISTS examples) # <- DO NOT EXPAND LIST
    set(cpp "${ex}.cpp")
    set(example "${ex}_example")
    add_executable(${example} ${cpp})
    target_include_directories(${example} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} obi ${BOOST_LIBRARIES})
    if(UNIX)
        target_link_libraries(${example} dl)
    endif(UNIX)
    separate_arguments(obi_stone-warnings)
    target_compile_options(${example} PRIVATE ${obi_stone-warnings})
endforeach()
