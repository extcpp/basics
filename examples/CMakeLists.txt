# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(obi_examples)

## required libs
find_package(Threads REQUIRED)

## examples
set(examples
    binary_index_tree
    container_helper
    function_cache
    functional
    if
    if_all_any
    load_library
    logging
    meta
    scope_guard
    scoped_timer
    show
    static_lambda
    tuple
    type_to_string
    vector
    serialize
)

foreach(ex IN LISTS examples) # <- DO NOT EXPAND LIST
    set(cpp "${ex}.cpp")
    set(example "${ex}_example")
    add_executable(${example} ${cpp})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} obi_shared)
    if(UNIX)
        target_link_libraries(${example} dl)
    endif(UNIX)
    target_compile_options(${example} PRIVATE ${obi_stone-warnings})
endforeach()

add_executable(any_printable_example any_printable_main.cpp any_printable_use_impl.cpp)
target_link_libraries(any_printable_example obi)
target_compile_options(any_printable_example PRIVATE ${obi_stone-warnings})
