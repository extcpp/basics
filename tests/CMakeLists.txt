project(libobi-tests)

set(test_name "libobi")
set(test_target "test_${test_name}")

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

if(OBI_CXX_FOLDS_AVAILABLE)
    list(APPEND test_files
        "meta_if_all_any"
    )
endif()

set(test_files_shared
    "logging"
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
    target_include_directories("${test_target_compile}" SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries("${test_target_compile}" obi${suffix} gtest_main gtest)
    target_compile_options("${test_target_compile}" PRIVATE ${obi_stone-warnings})
	add_test(NAME "${test_target_compile}_run" COMMAND $<TARGET_FILE:${test_target_compile}>)
    set_target_properties (${test_target_compile} PROPERTIES FOLDER tests/${test_target_compile})
endforeach()
