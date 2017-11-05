#! some basic settings i use really often so lets have them in a macro
macro(obi_setup)
    if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
        if(NOT OBI_SETUP_DONE)
            set(OBI_SETUP_DONE TRUE)
            if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
                if(UNIX)
                    set(CMAKE_INSTALL_PREFIX  "$ENV{HOME}/local")
                else()
                    message("not implemented for other operating systems")
                endif()
            endif()

            include(obi_in_compiler_warnings)
            #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${obi_stone-warnings}")

            set(OBI_CXX_COMPILER_IS_GCC FALSE)
            set(OBI_CXX_COMPILER_IS_CLANG FALSE)
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                set(OBI_CXX_COMPILER_IS_GCC TRUE)
                add_definitions(-DOBI_CLANG)
            elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                set(OBI_CXX_COMPILER_IS_CLANG TRUE)
                add_definitions(-DOBI_GCC)
            endif()

            # this must go
            if(OBI_CXX_COMPILER_IS_GCC OR OBI_CXX_COMPILER_IS_CLANG)
                set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -O0")
            endif()

            # replace with CMAKE_CXX_STANDARD when a newer cmake becomes available.
            if(OBI_CXX_COMPILER_IS_GCC AND
               CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6.0.0")
                message(STATUS "enable c++ 17")
                set(CMAKE_CXX_STANDARD 17)
                set(OBI_CXX_STANDARD 17)
                add_definitions(-DOBI_CXX_AVAILABLE=17)
            elseif(OBI_CXX_COMPILER_IS_CLANG AND
                   CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.0.0")
                message(STATUS "enable c++ 17")
                set(CMAKE_CXX_STANDARD 17)
                set(OBI_CXX_STANDARD 17)
                set(OBI_CXX_FOLDS_AVAILABLE TRUE)
                add_definitions(-DOBI_CXX_AVAILABLE=17)
                add_definitions(-DOBI_CXX_FOLDS_AVAILABLE)
            else()
                message(STATUS "enable c++ 14")
                set(CMAKE_CXX_STANDARD 14)
                set(OBI_CXX_STANDARD 14)
                add_definitions(-DOBI_CXX_AVAILABLE=14)
            endif()

            set(OBI_SETUP_DONE TRUE)
        endif()
    endif()
endmacro(obi_setup)

macro(obi_setup_with_test)
    obi_setup()
    include(CTest)
endmacro(obi_setup_with_test)

macro(obi_testing type)
    enable_testing()
    set(OBI_TEST_TYPE "${type}")

    set(dir "${ARGV1}")
    if(dir STREQUAL "")
        set(dir "tests")
    endif()

    if(CMAKE_TESTING)
        if("${type}" STREQUAL "google") # <-- expand type here!
            add_subdirectory(external_libs/googletest/googletest)
        elseif("${type}" STREQUAL "boost")
            if(NOT Boost_FOUND)
                find_package(Boost COMPONENTS unit_test_framework REQUIRED)
            endif()
        else()
            message(ERROR "unknown test type")
        endif()

        add_subdirectory("${dir}")
    endif()
## TODO
## add_Test(NAME "foo" COMMAND do_test arg --arg WORKING_DIRECTORY tests)
endmacro(obi_testing)

#! prefix string with provided symbol(s) until is has given length
#
#  in_string - sting to be prefixed
#  length - desired length
#  fill - symbols(s) used for filling
#  out_string - this will hold the result
function(obi_prefix in_string length fill out_string)
    set(result "${in_string}")
    string(LENGTH "${in_string}" current_length)

    while(current_length LESS length)
        set(result "${fill}${result}")
        string(LENGTH "${result}" current_length)
    endwhile()

    set("${out_string}" "${result}" PARENT_SCOPE)
endfunction(obi_prefix)

#! this function acts like add_subdirectory but it checks
#  if CMakeLists.txt exists in the directory before adding it
function(obi_add_subdirectory dir debug)
    if(EXISTS "${dir}/CMakeLists.txt")
        add_subdirectory("${dir}")
        if(debug)
            message("adding directory ${dir}")
        endif()
    endif()
endfunction()

macro(obi_install lib)
    install(
        TARGETS "${lib}"
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
    )

    set(args "${ARGN}")
    foreach(arg IN LISTS ${args})
        message("${arg}")
        install(
            DIRECTORY   "${arg}"
            DESTINATION include
        )
    endforeach()

    include(CPack)
endmacro(obi_install)
