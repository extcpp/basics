#pragma once

#include <string>
#include <map>
#include <vector>
#include <obi/util/cast.hpp>

namespace obi { namespace  util { namespace logging {

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
            logtopic(std::string&& name_, level trigger_level_)
                :name(std::move(name_))
                ,trigger_level(trigger_level_)
                {}
            logtopic(logtopic&& other) = default;
            logtopic(logtopic const& other) = default;

            std::string name;
            level trigger_level;
        };

        inline bool operator<(logtopic const& lhs, logtopic const& rhs){
            return lhs.name < rhs.name;
        }

        inline bool operator==(logtopic const& lhs, logtopic const& rhs){
            return lhs.name == rhs.name;
        }
    }

}}}
