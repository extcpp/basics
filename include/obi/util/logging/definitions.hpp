#pragma once
#ifndef OBI_UTIL_LOGGING_DEFINITIONS_HPP
#define OBI_UTIL_LOGGING_DEFINITIONS_HPP

#include <string>
#include <map>
#include <mutex>

namespace obi { namespace  util { namespace logging {
    namespace _detail {
        struct logtopic;
        extern std::mutex logmutex;
        extern std::map<int,logtopic*> topics_map;
    }

    enum class level : int {
        fatal = 0,
        error = 20,
        warn  = 40,
        info  = 60,
        debug = 80,
        trace = 100
    };

    namespace _detail {
        inline std::string const level_to_str(level level_){
            using namespace std::literals::string_literals;
            switch (level_) {
                case level::fatal:
                    return "fatal"s;
                case level::error:
                    return "error"s;
                case level::warn:
                    return "warning"s;
                case level::info:
                    return "info"s;
                case level::debug:
                    return "debug"s;
                case level::trace:
                    return "trace"s;
                default:
                    return "unknown"s;
            }
        }

        struct logtopic {
            logtopic(int id_, std::string&& name_, level trigger_level_)
                :id(id_)
                ,trigger_level(trigger_level_)
                ,name(std::move(name_))
                {
                    topics_map[id]=this;
                }

            logtopic(logtopic&& other) = default;
            logtopic(logtopic const& other) = default;

            int id;
            level trigger_level;
            std::string name;
        };

    } // _detail

    namespace configuration {
        extern std::map<int,_detail::logtopic*> topics;
        extern level global_level;
        // logging is configured globally via these varialbes
        // configure logging before you start logging!!!
        extern bool threads;
        extern bool filename;
        extern bool function;
        extern bool vim;
        extern bool gdb;
    }

    namespace topic{
        extern _detail::logtopic no_topic;
        extern _detail::logtopic network;
        extern _detail::logtopic engine;
    }

}}} // obi::util::logging
#endif // OBI_UTIL_LOGGING_DEFINITIONS_HPP
