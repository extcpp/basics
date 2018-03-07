// Copyright - 2018 - Jan Christoph Uhde <Jan@UhdeJC.com>
// requires c++17 (string_view)
#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace obi { namespace util {

//c++20 string has a member function starts_with
inline auto
starts_with(std::string_view const& str
           ,std::string_view const& prefix)
-> bool {
    if (str.size() < prefix.size()) return false;
    std::string_view str_short(str.data(),prefix.size());
    return str_short == prefix;
}

//c++20 string has a member function ends_with
inline auto
ends_with(std::string_view const& str
         ,std::string_view const& suffix)
-> bool {
    if (str.size() < suffix.size()) return false;
    std::size_t suffix_size = suffix.size();
    std::string_view str_short(str.data()+str.size()-suffix_size,suffix_size);
    return str_short == suffix;
}

template <typename StringType>
inline auto
split_on(std::string_view const& str
        ,std::string_view const& seq
        ,bool add_empty = false)
-> std::vector<StringType> {
    auto rv = std::vector<StringType>{};
    std::size_t start_pos = 0;
    auto len = str.size();

    while (start_pos < len) {
        std::size_t new_pos = str.find(seq,start_pos);
        if (new_pos == std::string::npos) break;
        std::size_t substr_len = new_pos - start_pos;
        if (substr_len || add_empty) {
            rv.emplace_back(str.substr(start_pos, substr_len));
        }
        start_pos = new_pos + seq.size();
    }

    std::size_t substr_len = len - start_pos;
    if (substr_len || add_empty) {
        rv.emplace_back(str.substr(start_pos, substr_len));
    }
    return rv;
}

}} // obi::util
