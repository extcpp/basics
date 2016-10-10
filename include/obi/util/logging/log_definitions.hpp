#pragma once

#include <string>
#include <map>
#include <vector>
#include <obi/util/cast.hpp>

namespace obi { namespace  util { namespace logging {

    enum class level : int {
        fatal = 0,
        error = 20,
        warn = 40,
        info = 60,
        debug = 80,
        trace = 100
    };

    std::string level_to_str(int level_){
        static const std::map<level,std::string> level_map =
        { {level::fatal, "fatal"}
        , {level::error, "error"}
        , {level::warn,  "warn"}
        , {level::info,  "info"}
        , {level::debug, "debug"}
        , {level::trace, "trace"}
        };

        auto current_level_ = level_map.begin();
        while(std::next(current_level_) != level_map.end()){
            if(obi::util::enum_to_underlying(std::next(current_level_)->first) > level_){
                break;
            }
            current_level_++;
        }
        return current_level_->second;
    }

    std::string level_to_str(level level_){
        return level_to_str(obi::util::enum_to_underlying(level_));
    }
}}}
