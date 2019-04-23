// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_LOGGING_FUNCTIONALITY_HEADER
#define OBI_UTIL_LOGGING_FUNCTIONALITY_HEADER

#include <obi/util/logging/definitions.hpp>
#include <obi/util/basic.hpp>
#include <obi/macros/platform.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace obi { namespace  util { namespace logging {
    namespace _detail {
        extern std::mutex logmutex;
        inline bool level_is_active(level level_, logtopic topic = topic::no_topic){
            return level_ <= topic.trigger_level;
        }

        struct logger {
            std::stringstream _ss;
            level _level;
            logger(logtopic const& topic, level level_,
                   const char* file_name, int line_no,
                   const char* function = "none");
            ~logger();

            template<typename T>
            void operator<< (T&& value){
                _ss << std::forward<T>(value);
            }
        };

    } // _detail

    void set_level_all(level level_){
        std::lock_guard<std::mutex> lock(_detail::logmutex);
        for(auto& topic : _detail::topics_map){
            topic.second->trigger_level=level_;
        }
    }

}}} // obi::util::logging
#endif // OBI_UTIL_LOGGING_FUNCTIONALITY_HEADER
