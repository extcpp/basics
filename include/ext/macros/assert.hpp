// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

#ifndef EXT_MACROS_ASSERT_HEADER
#define EXT_MACROS_ASSERT_HEADER

#include <ext/macros/compiler.hpp>
#include <iostream>

#ifdef EXT_CHECKED
    #define EXT_ASSERT(expr)                                              \
        do {                                                              \
            if (!(EXT_LIKELY(expr))) {                                    \
                std::cerr << "ERROR: EXT_ASSERT(" #expr ")" << std::endl; \
                std::abort();                                             \
            }                                                             \
        } while (0)
#else
    #define EXT_ASSERT(expr) \
        do {                 \
        } while (0)
#endif // EXT_CHECKED

#endif // EXT_MACROS_ASSERT_HEADER
