// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_LOGGING_FUNCTIONALITY_HEADER
#define OBI_LOGGING_FUNCTIONALITY_HEADER

#include <obi/logging/definitions.hpp>
#include <obi/util/basic.hpp>
#include <obi/macros/platform.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace obi { namespace logging {

    namespace _detail {
        inline bool level_is_active(level macro_level, logtopic topic = topic::no_topic){
            //activation_level 60(info) && macro_level 20 (error) -> log
            //activation_level 60(info) && macro_level 100(trace) -> no log
            // activation level must be greater than macro level
            return topic.activation_level >= macro_level;
        }

        constexpr inline bool default_level_is_active(level macro_level){
            return _detail::logtopic::default_level >= macro_level;
        }


        // this class does the real work it has to take care that messages
        // are not interleaved and it is reposible for organizing the output
        // stream, file, network
        //
        // TODO - provide different backends
        struct logger {
            std::stringstream _ss; // used to build up the log message
            std::ostream& _out;    // output - may change
            level _level;

            logger(char const* id, logtopic const& topic, level level_,
                   const char* file_name, int line_no,
                   const char* function = "none");
            ~logger();

            void write(); // may become virtual

            template<typename T>
            void operator<< (T&& value){
                _ss << std::forward<T>(value);
            }
        };

    } // _detail

    void set_level_all(level level_){
        std::lock_guard<std::mutex> lock(_detail::logmutex);
        for(auto& topic : _detail::topics_map){
            topic.second->activation_level=level_;
        }
    }

}} // obi::util::logging
#endif // OBI_LOGGING_FUNCTIONALITY_HEADER
