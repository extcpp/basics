project(test_package CXX)
cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
execute_process(COMMAND /bin/cat ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(ext-basics REQUIRED CONFIG)
add_executable(${PROJECT_NAME} test_package.cpp)
target_link_libraries(${PROJECT_NAME} ext::ext-basics)
