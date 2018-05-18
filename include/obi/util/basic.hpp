#pragma once
#ifndef OBI_UTIL_BASIC_HPP
#define OBI_UTIL_BASIC_HPP

#include <obi/macros/general.hpp>
#include <obi/util/cast.hpp>

#include <algorithm>
#include <string>
#include <type_traits>

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

inline std::string_view
filename(std::string const& pathname, bool is_linux = true, bool both = false){
    static const auto sep_predicate = [&](char c) {
        bool lin = ((both ||  is_linux) && c == '/');
        bool win = ((both || !is_linux) && c == '\\');
        return lin || win;
    };

    auto start_word_itr = std::find_if(pathname.rbegin()
                                      ,pathname.rend()
                                      ,sep_predicate
                                      ).base();
    auto start_pos = std::distance(pathname.begin(), start_word_itr);
    auto len = std::distance(start_word_itr, pathname.end());

    return std::string_view(pathname.data() + start_pos
                           ,::obi::util::to_unsigned(len)
                           );
}

}} // obi::util
#endif
