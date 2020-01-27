// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_FORWARD_STD_STRING_HEADER
#define EXT_UTIL_FORWARD_STD_STRING_HEADER

#if !defined(EXT_STL_LIBCXX) && !defined(EXT_STL_LIBSTDCXX) && !defined(EXT_STL_UNKNOWN)
    #ifdef _LIBCPP_VERSION
        #define EXT_STL_LIBCXX
    #elif defined(__GLIBCXX__)
        #define EXT_STL_LIBSTDCXX
    #elif defined(__has_include)
        #if __has_include(<bits/c++config.h>)
            #include <bits/c++config.h>
            #ifdef __GLIBCXX__
                #define EXT_STL_LIBSTDCXX
            #endif
        #endif
    #else
        #define EXT_STL_UNKNOWN
    #endif
#endif // !defined(EXT_STL_LIBCXX) && !defined(EXT_STL_LIBSTDCXX) && !defined(EXT_STL_UNKNOWN)

#ifdef EXT_STL_LIBSTDCXX
    #include <bits/stringfwd.h>
#elif defined(EXT_STL_LIBCXX)
    #include <iosfwd>
#else
    #include <string>
#endif // EXT_STL_LIBSTDCXX

#endif // EXT_UTIL_FORWARD_STD_STRING_HEADER
