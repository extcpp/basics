#! some basic settings i use really often so lets have them in a macro

function(ext_log)
    message(STATUS "extINFO -- " ${ARGV})
endfunction(ext_log)

function(ext_fatal)
    message(FATAL_ERROR "FATAL ERROR -- " ${ARGV})
endfunction(ext_fatal)

macro(ext_setup)
    # TODO - test this macro in other libs
    # execute macro only in top-level CMakeLists.txt
    if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
        # execute this setup just once
        if(NOT EXT_SETUP_DONE)
            if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
                set(LINUX TRUE)
            else()
                set(LINUX FALSE)
            endif()

            set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
            # set / modify default install prefix
            if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
                if(UNIX)
                    set(CMAKE_INSTALL_PREFIX  "$ENV{HOME}/local")
                else()
                    # do not change the default for other operating systems
                endif()
            endif()

            include(ext_cmake_compiler_warnings)

            set(EXT_CXX_COMPILER_IS_GCC FALSE)
            set(EXT_CXX_COMPILER_IS_CLANG FALSE)
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                set(EXT_CXX_COMPILER_IS_GCC TRUE)
            elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                set(EXT_CXX_COMPILER_IS_CLANG TRUE)
            endif()

            set(EXT_SETUP_DONE TRUE)
        endif()
    endif()
endmacro(ext_setup)

macro(ext_add_test_subdirectory type)
    set(EXT_TEST_TYPE "${type}")

    set(dir "${ARGV1}")
    if(dir STREQUAL "")
        set(dir "tests")
    endif()

    if(BUILD_TESTING) #activated by `include(CTest)`
        if("${type}" STREQUAL "google") # <-- expand type here!
            if(NOT TARGET gtest) #avoid recursive inclusion of gtest
              add_subdirectory(${LIBEXT_SOURCE_DIR}/external_libs/googletest)
            endif()
        else()
            message(ERROR "unknown test type")
        endif()
        ext_log("adding tests in: ${dir}")
        add_subdirectory("${dir}")
    endif()
endmacro(ext_add_test_subdirectory)

#! prefix string with provided symbol(s) until is has given length
#
#  in_string - sting to be prefixed
#  length - desired length
#  fill - symbols(s) used for filling
#  out_string - this will hold the result
function(ext_prefix in_string length fill out_string)
    set(result "${in_string}")
    string(LENGTH "${in_string}" current_length)

    while(current_length LESS length)
        set(result "${fill}${result}")
        string(LENGTH "${result}" current_length)
    endwhile()

    set("${out_string}" "${result}" PARENT_SCOPE)
endfunction(ext_prefix)

#! this function acts like add_subdirectory but it checks
#  if CMakeLists.txt exists in the directory before adding it
function(ext_add_subdirectory dir debug)
    if(EXISTS "${dir}/CMakeLists.txt")
        add_subdirectory("${dir}")
        if(debug)
            message("adding directory ${dir}")
        endif()
    endif()
endfunction()

macro(ext_install lib)
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
endmacro(ext_install)
