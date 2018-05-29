#! some basic settings i use really often so lets have them in a macro
macro(obi_setup)
    # TODO - test this macro in other libs
    # TODO - should not add definitions
    # execute macro only in top-level CMakeLists.txt
    if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
        # execute this setup just once
        if(NOT OBI_SETUP_DONE) 
            set(OBI_SETUP_DONE TRUE)

            # set / modify default install prefix
            if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
                if(UNIX)
                    set(CMAKE_INSTALL_PREFIX  "$ENV{HOME}/local")
                else()
                    # do not change the default for other operating systems
                endif()
            endif()

            include(obi_in_compiler_warnings)

            set(OBI_CXX_COMPILER_IS_GCC FALSE)
            set(OBI_CXX_COMPILER_IS_CLANG FALSE)
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                set(OBI_CXX_COMPILER_IS_GCC TRUE)
                add_definitions(-DOBI_CLANG)
            elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                set(OBI_CXX_COMPILER_IS_CLANG TRUE)
                add_definitions(-DOBI_GCC)
            endif()

            set(OBI_SETUP_DONE TRUE)
        endif()
    endif()
endmacro(obi_setup)

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
