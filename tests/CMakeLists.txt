project(libobi-tests)

set(test_name "libobi")
set(test_target "test_${test_name}")
set(test_files
    "ascii_string_helper"
    "cast"
    "math"
    "string_distances"
)


if("${OBI_TEST_TYPE}" STREQUAL "boost")
    #builds executable for every test
    foreach(test_name IN LISTS test_files) # <- DO NOT EXPAND LIST
        set(test_target "test_${test_name}")
        add_executable("${test_target}" "${test_target}.cpp")
        target_include_directories("${test_target}" PRIVATE ${Boost_INCLUDE_DIRS})
        target_link_libraries("${test_target}" libobi "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
        target_compile_definitions("${test_target}" PRIVATE -DBOOST_TEST_DYN_LINK)
        add_test("${test_target}_run" "${test_target}")
    endforeach()

elseif("${OBI_TEST_TYPE}" STREQUAL "google")
    #build one executable
    foreach(test_name IN LISTS test_files) # <- DO NOT EXPAND LIST
        list(APPEND test_files_cpp "test_${test_name}.cpp")
    endforeach()

    set(test_main  "gtest.cpp")
    add_executable("${test_target}" ${test_main} ${test_files_cpp})
    target_include_directories("${test_target}" PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries("${test_target}" obi gtest_main gtest)
    add_test("${test_target}_run" "${test_target}")

else()

    message(FATAL_ERROR "No test type given")

endif()
