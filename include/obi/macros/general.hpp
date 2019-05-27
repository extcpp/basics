// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MACROS_GENERAL_HEADER
#define OBI_MACROS_GENERAL_HEADER

#define OBI_NO_IMPL "NOT IMPLEMENTED NOW PLEASE FAIL HERE"
#define OBI_NOTHING(...)

#define OBI_DELETE(...)
#define OBI_EXPAND(...) __VA_ARGS__
#define OBI_STRINGIFY(a) #a

// use indirection (_OBI_CONCATENATE_IMPL) because ## inhibits expansion
#define _OBI_CONCATENATE_IMPL(s1, ...) s1 ## __VA_ARGS__
#define OBI_CONCATENATE(s1, ...) _OBI_CONCATENATE_IMPL(s1, __VA_ARGS__)

// __COUNTER__ provides increasing non negative number
#ifdef __COUNTER__
    #define OBI_ANONYMOUS_VARIABLE(str) OBI_CONCATENATE(str, __COUNTER__)
#else
    #define OBI_ANONYMOUS_VARIABLE OBI_NO_IMPL
#endif // __COUNTER__

#endif // OBI_MACROS_GENERAL_HEADER
