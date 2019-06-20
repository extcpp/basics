// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MACROS_ASSERT_HEADER
#define OBI_MACROS_ASSERT_HEADER

#ifdef OBI_CHECKED
    #define OBI_ASSERT(expr)       \
    do {                           \
        if (!(OBI_LIKELY(expr))) { \
            std::abort();          \
        }                          \
    } while (0)
#else
    #define OBI_ASSERT(expr) \
    do { } while (0)
#endif // OBI_CHECKED

#endif // OBI_MACROS_ASSERT_HEADER
