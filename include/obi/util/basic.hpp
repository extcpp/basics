#pragma once
#ifndef OBI_UTIL_BASIC_HPP
#define OBI_UTIL_BASIC_HPP

#include <string>
#include <algorithm>
#include <obi/macros/general.hpp>
#include <iostream>
namespace obi { namespace util {

template<typename F, typename ...T>
void for_each_arg(F&& function, T&&... args){
    (function(std::forward<T>(args)), ...); // c++17 fold expression
    //OBI_EXPAND_SIDE_EFFECTS(function(std::forward<T>(args)));
}

template<typename ...T>
void sort_all(T&&... args){
    static auto do_it = [](auto& container) { std::sort(std::begin(container), std::end(container)); };
    for_each_arg(do_it, std::forward<T>(args) ...);
}

inline std::string
basename(std::string const& pathname, bool is_linux = true, bool both = false){
    return std::string(std::find_if(pathname.rbegin()
                                   ,pathname.rend()
                                   ,[is_linux, both](char c) {
                                        return (
                                            ((both || is_linux)  && c == '/') ||
                                            ((both || !is_linux) && c == '\\')
                                        );
                                    }
                                   ).base()
                      ,pathname.end());
}
}} // obi::util
#endif
