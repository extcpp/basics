# Copyright - 2015-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.5.0)
project(ext_examples)

## required libs
find_package(Threads REQUIRED)

## examples
set(examples
    binary_index_tree
    container_helper
    if_all_any
    load_library
    meta
    scope_guard
    scoped_timer
    show
    tagged_pointer
    type_to_string
)

foreach(ex IN LISTS examples) # <- DO NOT EXPAND LIST
    set(cpp "${ex}.cpp")
    set(example "${ex}_example")
    add_executable(${example} ${cpp})
    target_link_libraries(${example}
        ${CMAKE_THREAD_LIBS_INIT}
        ext::basics
    )
    if(UNIX)
        target_link_libraries(${example} dl)
    endif(UNIX)
    target_compile_options(${example} PRIVATE ${ext_stone-warnings})
    set_target_properties (${example} PROPERTIES FOLDER examples/${example})
endforeach()
