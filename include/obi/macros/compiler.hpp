// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MACROS_COMPILER_HEADER
#define OBI_MACROS_COMPILER_HEADER
#include <obi/macros/general.hpp>

#ifdef __GNUC__
    //gcc or clang
    #define OBI_COMPILER_GNU 1
    #define OBI_DO_PRAGMA(x) _Pragma (#x)
#elif defined _MSC_VER
    //visual studio
    #define OBI_COMPILER_VC 1
    #define OBI_DO_PRAGMA(x) __pragma (x)
#endif // __GNUC__

// Warnings
#ifdef OBI_COMPILER_GNU
    #define OBI_WARNING(msg) OBI_DO_PRAGMA(GCC warning #msg)
#elif defined OBI_COMPILER_VC
    #define OBI_WARNING(msg) OBI_DO_PRAGMA(message #msg)
#else
    #define OBI_WARNING(msg) OBI_NO_IMPL
#endif // OBI_COMPILER_GNU

#ifdef OBI_COMPILER_GNU
    #define OBI_ALWAYS_INLINE inline __attribute__((__always_inline__))
    #define OBI_NEVER_INLINE __attribute__((__no_inline__))
    #define OBI_FALLTHROUGH __attribute__((__fallthrough__))
    #define OBI_UNUSED __attribute__((__unused__))
    #define OBI_HOT  __attribute__((__hot__))
    #define OBI_COLD __attribute__((__cold__))
#elif defined OBI_COMPILER_VC
    #define OBI_ALWAYS_INLINE __forceinline
    #define OBI_NEVER_INLINE  __declspec(noinline)
    #define OBI_FALLTHROUGH OBI_NOTHING
    #define OBI_UNUSED OBI_NOTHING
    #define OBI_HOT    OBI_NOTHING
    #define OBI_COLD   OBI_NOTHING
#else
    #define OBI_ALWAYS_INLINE OBI_NOTHING
    #define OBI_NEVER_INLINE OBI_NOTHING
    #define OBI_FALLTHROUGH OBI_NOTHING
    #define OBI_UNUSED OBI_NOTHING
    #define OBI_HOT    OBI_NOTHING
    #define OBI_COLD   OBI_NOTHING
#endif // OBI_COMPILER_GNU


// Dynamic initizlization of static vars
#ifdef OBI_COMPILER_GNU
    #define OBI_INIT_PRIORITY_GNU(prio) __attribute__((__init_priority__((prio))))
    //rather use manual when you know what you do
    #define OBI_INIT_PRIORITY_GNU_HIGH OBI_INIT_PRIORITY_GNU(101)
    #define OBI_INIT_PRIORITY_GNU_LOW  OBI_INIT_PRIORITY_GNU(65535)
#else
    #define OBI_INIT_PRIORITY_GNU(prio) OBI_NOTHING
    #define OBI_INIT_PRIORITY_GNU_HIGH  OBI_NOTHING
    #define OBI_INIT_PRIORITY_GNU_LOW   OBI_NOTHING
#endif // OBI_COMPILER_GNU

#ifdef OBI_COMPILER_VC
    // init_set: https://msdn.microsoft.com/en-us/library/7977wcck.aspx
    #define OBI_INIT_PRIORITY_VC_HIGH OBI_DO_PRAGMA(init_seg, lib)
    #define OBI_INIT_PRIORITY_VC_LOW  OBI_DO_PRAGMA(init_seg, user)
#else
    #define OBI_INIT_PRIORITY_VC_HIGH OBI_NOTHING
    #define OBI_INIT_PRIORITY_VC_LOW  OBI_NOTHING
#endif // OBI_COMPILER_VC

#endif // OBI_MACROS_COMPILER_HEADER
