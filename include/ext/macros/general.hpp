// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_MACROS_GENERAL_HEADER
#define EXT_MACROS_GENERAL_HEADER

#define EXT_NO_IMPL "NOT IMPLEMENTED NOW PLEASE FAIL HERE"
#define EXT_NOTHING(...)

#define EXT_DELETE(...)
#define EXT_EXPAND(...) __VA_ARGS__
#define EXT_STRINGIFY(a) #a

// use indirection (_EXT_CONCATENATE_IMPL) because ## inhibits expansion
#define _EXT_CONCATENATE_IMPL(s1, ...) s1##__VA_ARGS__
#define EXT_CONCATENATE(s1, ...) _EXT_CONCATENATE_IMPL(s1, __VA_ARGS__)

// __COUNTER__ provides increasing non negative number
#ifdef __COUNTER__
    #define EXT_ANONYMOUS_VARIABLE(str) EXT_CONCATENATE(str, __COUNTER__)
#else
    #define EXT_ANONYMOUS_VARIABLE EXT_NO_IMPL
#endif // __COUNTER__

#endif // EXT_MACROS_GENERAL_HEADER
