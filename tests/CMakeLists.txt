project(libobi-tests)

set(test_name "libobi")
set(test_target "test_${test_name}")

set(test_files 
    "algorithm_string_distances"
    "math"
    "meta_has_member"
    "meta_if"
    "meta_is_one_of"
    "util_ascii_string_helper"
    "util_cast"
    "util_result"
)

if(OBI_CXX_FOLDS_AVAILABLE)
    list(APPEND test_files
        "meta_if_all_any"
    )
endif()

set(test_files_shared
    #"logging"
)

foreach(suffix IN ITEMS "" "_shared")
    #build one executable
    set(test_files_cpp)
    foreach(test_name IN LISTS test_files${suffix}) # <- DO NOT EXPAND LIST
        list(APPEND test_files_cpp "${test_name}.cpp")
    endforeach()

    set(test_target_compile "${test_target}${suffix}")
    set(test_main  "gtest.cpp")
    add_executable("${test_target_compile}" ${test_main} ${test_files_cpp})
    target_include_directories("${test_target_compile}" PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries("${test_target_compile}" obi gtest_main gtest)
    add_test("${test_target_compile}_run" "${test_target_compile}")
endforeach()
