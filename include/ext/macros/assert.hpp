// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_MACROS_ASSERT_HEADER
#    define EXT_MACROS_ASSERT_HEADER

#    ifdef EXT_CHECKED
#        define EXT_ASSERT(expr)           \
            do {                           \
                if (!(EXT_LIKELY(expr))) { \
                    std::abort();          \
                }                          \
            } while (0)
#    else
#        define EXT_ASSERT(expr) \
            do {                 \
            } while (0)
#    endif // EXT_CHECKED

#endif // EXT_MACROS_ASSERT_HEADER
