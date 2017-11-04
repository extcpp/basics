# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(obi_examples)

## required libs
find_package(Threads)

## examples

set(examples
    binary_index_tree
    compile_check
    concurrency
    container_helper
    function_cache
    functional
    if
    load_library
    meta
    scoped_timer
    show
    static_lambda
    type_to_string
    vector
    any_printable
    logging
)

if(OBI_CXX_STANDARD EQUAL 17)
    list(APPEND examples
        tuple
        scope_guard
    )
endif()

if(OBI_CXX_FOLDS_AVAILABLE)
    list(APPEND examples
        if_all_any
    )
endif()

foreach(ex IN LISTS examples) # <- DO NOT EXPAND LIST
    set(cpp "${ex}.cpp")
    set(example "${ex}_example")
    add_executable(${example} ${cpp})
    target_include_directories(${example} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} obi_shared ${BOOST_LIBRARIES})
    if(UNIX)
        target_link_libraries(${example} dl)
    endif(UNIX)
    separate_arguments(obi_stone-warnings)
    target_compile_options(${example} PRIVATE ${obi_stone-warnings})
endforeach()
