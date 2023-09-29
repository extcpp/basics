if (NOT TARGET ext::foundation)
    message(STATUS "[extcpp] trying to find ext::foundation")
    message(STATUS "[extcpp] EXT_DOWNLOAD=${EXT_DOWNLOAD}")
    if(EXT_DOWNLOAD)
        message(STATUS "[extcpp] declaring download location for ext::foundation")
        include(FetchContent)
        FetchContent_Declare(
            ExtFoundation
            GIT_REPOSITORY https://github.com/extcpp/foundation.git
            GIT_TAG        master
        )
        FetchContent_MakeAvailable(ExtFoundation)
    elseif(EXISTS "${EXT_FOUNDATION_PATH}/CMakeLists.txt")
        message(STATUS "[extcpp] looking for foundation in ${EXT_FOUNDATION_PATH}")
        add_subdirectory("${EXT_FOUNDATION_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/foundation")
    endif()

    if(NOT TARGET ext::foundation)
        find_package(ExtFoundation REQUIRED) # already fatal
        message(FATAL_ERROR "[extcpp] can not provide target ext::foundation")
    endif()
endif()
