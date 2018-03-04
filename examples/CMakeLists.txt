# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(obi_examples)

## required libs
find_package(Threads)

## examples

set(examples
    binary_index_tree
    container_helper
    function_cache
    functional
    if
    load_library
    logging
    meta
    scoped_timer
    show
    static_lambda
    type_to_string
    vector
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

separate_arguments(obi_stone-warnings)

foreach(ex IN LISTS examples) # <- DO NOT EXPAND LIST
    set(cpp "${ex}.cpp")
    set(example "${ex}_example")
    add_executable(${example} ${cpp})
    target_include_directories(${example} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${example} ${CMAKE_THREAD_LIBS_INIT} obi_shared ${BOOST_LIBRARIES})
    if(UNIX)
        target_link_libraries(${example} dl)
    endif(UNIX)
    target_compile_options(${example} PRIVATE ${obi_stone-warnings})
endforeach()


add_executable(any_printable_example any_printable_main.cpp any_printable_use_impl.cpp)
target_link_libraries(any_printable_example obi)
target_compile_options(any_printable_example PRIVATE ${obi_stone-warnings})
