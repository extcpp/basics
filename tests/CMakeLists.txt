project(libobi-tests)

set(test_files 
    "algorithm_string_distances"
    "math"
    "meta_has_member"
    "meta_if"
    "meta_is_one_of"
    "util_any_printable"
    "util_basic"
    "util_cast"
    "util_encode"
    "util_endian"
    "util_result"
    "util_serialization"
    "util_string"
    "util_windows_strings"
    "util_except"
)

set(test_files_shared
    "logging"
)

if(OBI_CXX_FOLDS_AVAILABLE)
list(APPEND test_files
    "meta_if_all_any"
)
endif()

foreach(suffix IN ITEMS "" "_shared")
    #build one executable
    set(test_sources)
    foreach(test_name IN LISTS test_files${suffix}) # <- DO NOT EXPAND LIST
        list(APPEND test_sources "${test_name}.cpp")
    endforeach()

    set(test_target "test_libobi${suffix}")
    add_executable("${test_target}" gtest.cpp ${test_sources})
    target_include_directories("${test_target}" SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries("${test_target}" obi${suffix} gtest_main gtest)
    target_compile_options("${test_target}" PRIVATE ${obi_stone-warnings})
	add_test(NAME "${test_target}_run" COMMAND $<TARGET_FILE:${test_target}>)
    set_target_properties (${test_target} PROPERTIES FOLDER tests/${test_target})
endforeach()
