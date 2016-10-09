#pragma once

#include <string>
#include <map>
#include <vector>
#include <obi/util/cast.hpp>

namespace obi::util::logging {

    enum class log_level : int {
        fatal = 0,
        error = 20,
        warn = 40,
        info = 60,
        debug = 80,
        trace = 100
    };

    std::string level_to_str(int level){
        static const std::map<log_level,std::string> level_map =
        { {log_level::fatal, "fatal"}
        , {log_level::error, "error"}
        , {log_level::warn,  "warn"}
        , {log_level::info,  "info"}
        , {log_level::debug, "debug"}
        , {log_level::trace, "trace"}
        };

        auto current_level = level_map.begin();
        while(std::next(current_level) != level_map.end()){
            if(obi::util::enum_to_underlying(std::next(current_level)->first) > level){
                break;
            }
            current_level++;
        }
        return current_level->second;
    }

    std::string level_to_str(log_level level){
        return level_to_str(obi::util::enum_to_underlying(level));
    }
}
