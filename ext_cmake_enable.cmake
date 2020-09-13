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
    if(EXISTS ${ext_cmake_dir}/ext_script_git_version.cmake)
        message(STATUS "extINFO -- found ext cmake directory in: ${ext_cmake_dir}")
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
