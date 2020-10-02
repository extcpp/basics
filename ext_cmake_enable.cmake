#TODO use version parameter to ensure a certaion version
if(EXT_CMAKE_ENABLED)
    return()
endif()

#try some common paths
set(ext_cmake_dir_hints
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/../cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/../basics/cmake"
    "${EXT_CMAKE_PATH}"
    "${EXT_LIBRARIES_PATH}/cmake"
    "${EXT_LIBRARIES_PATH}/basics/cmake"
)

foreach(ext_cmake_dir_candidate IN LISTS EXT_CMAKE_HINTS EXT_CMAKE_PATHS ext_cmake_dir_hints)
    set(ext_cmake_dir "${ext_cmake_dir_candidate}")
    get_filename_component(ext_cmake_dir "${ext_cmake_dir}" ABSOLUTE)
    if(EXISTS ${ext_cmake_dir}/ext_script_git_version.cmake)
        message(STATUS "extINFO -- Found ext cmake directory in: ${ext_cmake_dir}")

        set(ext_libraries_path_candidate "${ext_cmake_dir}/..")
        get_filename_component(ext_libraries_path_candidate "${ext_libraries_path_candidate}" ABSOLUTE)

        if(NOT EXT_LIBRARIES_PATH)
            if(EXISTS "${ext_libraries_path_candidate}/basics/include/ext/util")
                message(STATUS "extINFO -- Setting EXT_LIBRARIES_PATH to: ${ext_libraries_path_candidate} (enable_cmake)")
                set(EXT_LIBRARIES_PATH "${ext_libraries_path_candidate}" CACHE PATH "Path of ext libraries.")
            endif()
        endif()
        break()
    endif()
endforeach()

#download
if(NOT EXISTS ${ext_cmake_dir}/ext_script_git_version.cmake)
    message(STATUS "extINFO -- downloading extcpp/cmake")
    #if there is a cmake directory just use that
    include(FetchContent)
    FetchContent_Declare(ext_cmake_github
        GIT_REPOSITORY https://github.com/extcpp/cmake.git
        GIT_TAG master
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(ext_cmake_github)
    message("downloaded ext/cmake to: ${ext_cmake_github_SOURCE_DIR}")
    set(ext_cmake_dir "${ext_cmake_github_SOURCE_DIR}")
endif()

#final check and include
if(NOT EXISTS ${ext_cmake_dir}/ext_script_git_version.cmake)
    message(FATAL_ERROR "Could not locate extcpp/cmake directory!")
else()
    list(APPEND CMAKE_MODULE_PATH "${ext_cmake_dir}")
endif()

message(STATUS "extINFO -- CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

set(EXT_CMAKE_ENABLED TRUE)
