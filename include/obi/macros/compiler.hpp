#pragma once
#ifndef OBI_MACROS_COMPILER_HPP
#define OBI_MACROS_COMPILER_HPP
#include <obi/macros/general.hpp>

#ifdef __GNUC__
    //gcc or clang
    #define OBI_COMPILER_GNU
    #define OBI_DO_PRAGMA(x) _Pragma (#x)
#elif _MSC_VER
    //visual studio
    #define OBI_COMPILER_VC
    #define OBI_DO_PRAGMA(x) __pragma (x)
#endif

// Warnings
#ifdef OBI_COMPILER_GNU
    #define OBI_WARNING(msg) OBI_DO_PRAGMA(GCC warning #msg)
#elif OBI_COMPILER_VC
    #define OBI_WARNING(msg) OBI_DO_PRAGMA(message #msg)
#else
    #define OBI_WARNING(msg) OBI_NO_IMPL
#endif


// Dynamic initizlization of static vars
#ifdef OBI_COMPILER_GNU
    #define OBI_INIT_PRIORITY_GNU(prio) __attribute__((init_priority((prio))))
    //rather use manual when you know what you do
    #define OBI_INIT_PRIORITY_GNU_HIGH __attribute__((init_priority((101))))
    #define OBI_INIT_PRIORITY_GNU_LOW  __attribute__((init_priority((65535))))
#else
    #define OBI_INIT_PRIORITY_GNU(prio)
    #define OBI_INIT_PRIORITY_GNU_HIGH
    #define OBI_INIT_PRIORITY_GNU_LOW
#endif

#ifdef OBI_COMPILER_VC
    // init_set: https://msdn.microsoft.com/en-us/library/7977wcck.aspx
    #define OBI_INIT_PRIORITY_VC_HIGH OBI_DO_PRAGMA(init_seg, lib)
    #define OBI_INIT_PRIORITY_VC_LOW  OBI_DO_PRAGMA(init_seg, user)
#else
    #define OBI_INIT_PRIORITY_VC_HIGH
    #define OBI_INIT_PRIORITY_VC_LOW
#endif

#endif // OBI_MACROS_COMPILER_HPP
