// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_MACROS_PLATFORM_HEADER
#define EXT_MACROS_PLATFORM_HEADER
#include "compiler.hpp"

#ifdef __linux__
    #define EXT_LINUX
#endif // __linux__

#ifdef __unix__
    #define EXT_UNIX
#elif defined _WIN32
    #define EXT_WINDOWS
#endif // __unix__

#ifdef __APPLE__
    #define EXT_MACOS
#endif // __APPLE__

// arch
#ifdef EXT_UNIX
    #ifdef __amd64__
        #define EXT_X64
    #endif // __amd64__
#elif defined EXT_WINDOWS
    #ifdef _WIN64
        #define EXT_X64
    #else // check for others like arm
        #define EXT_X32
    #endif // _WIN64
#endif     // EXT_UNIX

#endif // EXT_MACROS_PLATFORM_HEADER
