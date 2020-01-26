// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
// get gcc macros
// gcc -dM -E - < /dev/null
#ifndef EXT_CONFIG_HEADER
#define EXT_CONFIG_HEADER

#ifdef __linux
    // define fallback for linux if cmak is not able to figure out
    // FIXME - add architecture check
    #ifndef EXT_KNOWN_L1_CACHE_LINE_SIZE
        #define EXT_KNOWN_L1_CACHE_LINE_SIZE 64
    #endif // EXT_KNOWN_L1_CACHE_LINE_SIZE
#endif     // __linux

#endif // EXT_CONFIG_HEADER
