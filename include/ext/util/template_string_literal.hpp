// Copyright - xxxx-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_TEMPLATE_STRING_LITERAL_HEADER
#define EXT_UTIL_TEMPLATE_STRING_LITERAL_HEADER
#include <algorithm>

namespace ext::util {

// Usage:
// template<template_string_literal lit>
// struct example_class {
//     const std::string name  = lit.value;
// };

// c++20 -- class types in non-type template parameters
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0732r2.pdf
template <std::size_t N>
struct template_string_literal {
    constexpr template_string_literal(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
};

} // namespace ext::util
#endif // EXT_UTIL_TEMPLATE_STRING_LITERAL_HEADER
