// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_IO_HEADER
#define EXT_UTIL_IO_HEADER
#include <ext/config.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>


namespace ext { namespace util {

inline std::pair<std::string, bool> istream_to_string(std::istream& in, bool remove_spaces = false) {
    constexpr std::size_t buf_size = 4096;

    std::string result;
    std::string::value_type buffer[buf_size];

    while (in.read(buffer, sizeof(buffer))) {
        result.append(buffer, sizeof(buffer));
    }

    // number of chars in last operation
    result.append(buffer, in.gcount());

    if (remove_spaces) {
        auto is_space = [](unsigned char const c) {
            return std::isspace(c);
        };
        result.erase(std::remove_if(result.begin(), result.end(), is_space), result.end());
    }

    return {result, in.eof()};
}

inline std::pair<std::string, bool> ifstream_to_string(std::ifstream& in, bool remove_spaces = false) {
    if (!in.is_open()) {
        throw std::logic_error("You try to read from a closed stream!");
    }
    return istream_to_string(in, remove_spaces);
}
}}     // namespace ext::util
#endif // EXT_UTIL_IO_HEADER
