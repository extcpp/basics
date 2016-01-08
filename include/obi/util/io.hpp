// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <iostream>
#include <string>
#include <algorithm>

namespace obi { namespace util {

    std::string stream_to_string(std::istream &in, bool remove_spaces = false) {
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

}}
