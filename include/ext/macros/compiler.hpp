// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_MACROS_COMPILER_HEADER
#    define EXT_MACROS_COMPILER_HEADER
#    include <ext/macros/general.hpp>

#    ifdef __GNUC__
// gcc or clang
#        define EXT_COMPILER_GNU 1
#        define EXT_DO_PRAGMA(x) _Pragma(#        x)
#        define EXT_EXPORT_VC EXT_NOTHING()
#    elif defined _MSC_VER
// visual studio
#        define EXT_COMPILER_VC 1
#        define EXT_DO_PRAGMA(x) __pragma(x)
#        ifdef EXT_IN_LIB
#            define EXT_EXPORT_VC __declspec(dllexport)
#        else
#            define EXT_EXPORT_VC __declspec(dllimport)
#        endif
#    endif // __GNUC__

// Warnings
#    ifdef EXT_COMPILER_GNU
#        define EXT_WARNING(msg) EXT_DO_PRAGMA(GCC warning #        msg)
#    elif defined EXT_COMPILER_VC
#        define EXT_WARNING(msg) EXT_DO_PRAGMA(message #        msg)
#    else
#        define EXT_WARNING(msg) EXT_NO_IMPL
#    endif // EXT_COMPILER_GNU

#    ifdef EXT_COMPILER_GNU
#        define EXT_ALWAYS_INLINE inline __attribute__((__always_inline__))
#        define EXT_NEVER_INLINE __attribute__((__no_inline__))
#        define EXT_UNUSED __attribute__((__unused__))
#        define EXT_HOT __attribute__((__hot__))
#        define EXT_COLD __attribute__((__cold__))
#        define EXT_FALLTHROUGH __attribute__((__fallthrough__))
#        define EXT_LIKELY(x) __builtin_expect(!!(x), 1)
#        define EXT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#    elif defined EXT_COMPILER_VC
#        define EXT_ALWAYS_INLINE __forceinline
#        define EXT_NEVER_INLINE __declspec(noinline)
#        define EXT_UNUSED EXT_NOTHING()
#        define EXT_HOT EXT_NOTHING()
#        define EXT_COLD EXT_NOTHING()
#        define EXT_FALLTHROUGH EXT_NOTHING()
#        define EXT_LIKELY(x) (x)
#        define EXT_UNLIKELY(x) (x)
#    else
#        define EXT_ALWAYS_INLINE EXT_NOTHING()
#        define EXT_NEVER_INLINE EXT_NOTHING()
#        define EXT_FALLTHROUGH EXT_NOTHING()
#        define EXT_UNUSED EXT_NOTHING()
#        define EXT_HOT EXT_NOTHING()
#        define EXT_COLD EXT_NOTHING()
#        define EXT_LIKELY(x) (x)
#        define EXT_UNLIKELY(x) (x)
#    endif // EXT_COMPILER_GNU

// Dynamic initizlization of static vars
#    ifdef EXT_COMPILER_GNU
#        define EXT_INIT_PRIORITY_GNU(prio) __attribute__((__init_priority__((prio))))
// rather use manual when you know what you do
#        define EXT_INIT_PRIORITY_GNU_HIGH EXT_INIT_PRIORITY_GNU(101)
#        define EXT_INIT_PRIORITY_GNU_LOW EXT_INIT_PRIORITY_GNU(65535)
#    else
#        define EXT_INIT_PRIORITY_GNU(prio) EXT_NOTHING()
#        define EXT_INIT_PRIORITY_GNU_HIGH EXT_NOTHING()
#        define EXT_INIT_PRIORITY_GNU_LOW EXT_NOTHING()
#    endif // EXT_COMPILER_GNU

#    ifdef EXT_COMPILER_VC
// init_set: https://msdn.microsoft.com/en-us/library/7977wcck.aspx
#        define EXT_INIT_PRIORITY_VC_HIGH EXT_DO_PRAGMA(init_seg, lib)
#        define EXT_INIT_PRIORITY_VC_LOW EXT_DO_PRAGMA(init_seg, user)
#    else
#        define EXT_INIT_PRIORITY_VC_HIGH EXT_NOTHING()
#        define EXT_INIT_PRIORITY_VC_LOW EXT_NOTHING()
#    endif // EXT_COMPILER_VC

#endif // EXT_MACROS_COMPILER_HEADER
