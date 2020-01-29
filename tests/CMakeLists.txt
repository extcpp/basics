project(ext-basics-tests)

set(test-files 
    "algorithm_basic"
    "algorithm_string_distances"
    "algorithm_knapsack"
    "math"
    "meta_has_member"
    "meta_is_one_of"

    "structures_binary_index_tree"

    "util_any_printable"
    "util_basic"
    "util_cast"
    "util_lazy"
    "util_encode"
    "util_endian"
    "util_enum"
    "util_except"
    "util_result"
    "util_scoped_timer"
    "util_serialization"
    "util_show"
    "util_string"
    "util_windows_strings"

    "expected/assignment"
    "expected/bases"
    #"expected/constexpr"
    "expected/constructors"
    "expected/emplace"
    "expected/extensions"
    "expected/issues"
    #"expected/main"
    #"expected/noexcept"
    "expected/observers"
    #"expected/relops"
    "expected/swap"
    #"expected/test"
)

if(EXT_CXX_FOLDS_AVAILABLE)
list(APPEND test-files
    "meta_if_all_any"
)
endif()

foreach(suffix IN ITEMS "")
    #build one executable
    set(test_sources)
    foreach(test_name IN LISTS test-files${suffix}) # <- DO NOT EXPAND LIST
        list(APPEND test_sources "${test_name}.cpp")
    endforeach()

    set(test_target "test-ext-basics${suffix}")
    add_executable("${test_target}" gtest.cpp ${test_sources})
    target_include_directories("${test_target}" SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries("${test_target}" ext::basics${suffix} gtest_main gtest Threads::Threads)
    target_compile_options("${test_target}" PRIVATE ${ext_stone-warnings})
    target_compile_definitions("${test_target}" PUBLIC EXT_CHECKED=1 EXT_IN_TEST=1)
    # -- repeated calls should append which does not happen for me (cmake 3.16 on linux)
    #target_compile_definitions("${test_target}" PUBLIC EXT_IN_TEST=1
	add_test(NAME "${test_target}_run" COMMAND $<TARGET_FILE:${test_target}>)
    set_target_properties (${test_target} PROPERTIES FOLDER tests/${test_target})
endforeach()



add_executable(expected_standalone_test_swap expected/standalone_test_swap.cpp)
target_link_libraries(expected_standalone_test_swap ext::basics)
target_compile_options(expected_standalone_test_swap PRIVATE ${ext_stone-warnings})
target_compile_definitions(expected_standalone_test_swap PUBLIC EXT_CHECKED=1 EXT_IN_TEST=1)
add_test(NAME expected_standalone_test_swap_run COMMAND $<TARGET_FILE:expected_standalone_test_swap>)
set_target_properties (expected_standalone_test_swap PROPERTIES FOLDER tests/expected_standalone_test_swap)
