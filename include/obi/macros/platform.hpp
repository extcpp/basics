#pragma once
#ifndef OBI_MACROS_PLATFORM_HPP
#define OBI_MACROS_PLATFORM_HPP
#include "compiler.hpp"

#ifdef __linux___
    #define OBI_LINUX
#endif

#ifdef __unix___
    #define OBI_UNIX
#elif __WIN32
    #define OBI_WINDOWS
#endif

#ifdef OBI_UNIX
    #ifdef __amd64__
        #define OBI_X64
    #endif
#elif OBI_WINDOWS
    #ifdef _WIN64
        #define OBI_X64
    #else //check for others like arm
        #define OBI_X32
    #endif
#endif

#endif // OBI_MACROS_PLATFORM_HPP