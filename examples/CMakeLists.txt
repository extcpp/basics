# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

cmake_minimum_required(VERSION 3.0.0)
project(libobi_examples)

## required libs
find_package(Threads)

## examples
add_executable(functional functional.cpp)
target_link_libraries(functional libobi)


add_executable(function_cache function_cache.cpp)
target_link_libraries(function_cache libobi)
target_link_libraries(function_cache ${CMAKE_THREAD_LIBS_INIT})


add_executable(if if.cpp)
target_link_libraries(if libobi)


add_executable(load_library load_library.cpp)
target_link_libraries(load_library libobi)
if(UNIX)
    target_link_libraries(load_library dl)
endif(UNIX)


add_executable(meta meta.cpp)
target_link_libraries(meta libobi)


#add_executable(multi_array multi_array.cpp)
#target_link_libraries(multi_array libobi)


add_executable(scoped_timer scoped_timer.cpp)
target_link_libraries(scoped_timer libobi)
target_link_libraries(scoped_timer ${CMAKE_THREAD_LIBS_INIT})


add_executable(static_lambda static_lambda.cpp)
target_link_libraries(static_lambda libobi)


add_executable(type_to_string type_to_string.cpp)
target_link_libraries(type_to_string libobi)

add_executable(container_helper container_helper.cpp)
target_link_libraries(container_helper libobi)

add_executable(vector vector.cpp)
target_link_libraries(vector libobi)

add_executable(concurrency concurrency.cpp)
target_link_libraries(concurrency libobi)

add_executable(scope_guard scope_guard.cpp)
target_link_libraries(scope_guard libobi)

add_executable(show show.cpp)
target_link_libraries(show libobi)
