#pragma once
#ifndef OBI_UTIL_BASIC_HPP
#define OBI_UTIL_BASIC_HPP

#include <string>
#include <algorithm>
#include <obi/macros/general.hpp>
#include <iostream>
namespace obi { namespace util {

template<typename ...T>
void sort_all(T&... args){
    auto do_it = [](auto& container) { std::sort(std::begin(container), std::end(container)); };
    //(do_it(args), ...); // c++17 fold expression
    OBI_EXPAND_SIDE_EFFECTS(do_it(args));

}

inline std::string
basename(std::string const& pathname){
    return std::string(std::find_if(pathname.rbegin()
                                   ,pathname.rend()
                                   ,[](char c) { return c == '/' || c =='\\';}
                                   ).base()
                      ,pathname.end());
}
}} // obi::util
#endif
