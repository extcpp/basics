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
    else()
        find_package(ExtFoundation REQUIRED)
    endif()

    if(NOT TARGET ext::foundation)
        message(FATAL_ERROR "[extcpp] can not provide target ext::foundation")
    endif()
endif()
