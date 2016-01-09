// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include "../config.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace obi { namespace util {

    OBI_INLINE std::string
    istream_to_string(std::istream &in, bool remove_spaces = false) {
        std::string result;
        char buffer[4096];
        while (in.read(buffer, sizeof(buffer))) {
            result.append(buffer, sizeof(buffer));
        }
        //number of chars in last operation
        result.append(buffer, in.gcount());

        if(remove_spaces) {
            auto is_space = [](unsigned char const c) { return std::isspace(c); };
            result.erase(std::remove_if(result.begin()
                                       ,result.end()
                                       ,is_space)
                        ,result.end());
        }
        return result;
    }

    OBI_INLINE std::string
    ifstream_to_string(std::ifstream &in, bool remove_spaces = false) {
        if(! in.is_open()) {
            throw std::logic_error("You try to read from a closed stream!");
        }
        return istream_to_string(in,remove_spaces);
    }

}}
