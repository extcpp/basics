// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MACROS_GENERAL_HEADER
#define OBI_MACROS_GENERAL_HEADER

#define OBI_NO_IMPL "NOT IMPLEMENTED NOW PLEASE FAIL HERE"
#define OBI_NOTHING
#define OBI_STRINGIFY(a) #a
#define OBI_CONCATENATE_IMPL(s1, s2) s1##s2
#define OBI_CONCATENATE(s1, s2) OBI_CONCATENATE_IMPL(s1, s2)


// __COUNTER__ provides increasing non negative number
#ifdef __COUNTER__
    #define OBI_ANONYMOUS_VARIABLE(str) OBI_CONCATENATE(str, __COUNTER__)
#else
    #define OBI_NO_IMPL
#endif // __COUNTER__

// not required since c++17
namespace obi { namespace macros { namespace _detail{
        using expand_type = int[];
}}}
#define OBI_EXPAND_SIDE_EFFECTS(expression) int pass[]{ ((expression), 0)... }; (void)pass;


#endif // OBI_MACROS_GENERAL_HEADER
