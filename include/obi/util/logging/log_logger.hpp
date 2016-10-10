#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "log_definitions.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace obi { namespace  util { namespace logging {
    struct log_topic {
        log_topic(std::string&& n, level def, level trig) :
            name(std::move(n)), default_level_(def), trigger_level_(trig){}
        log_topic(log_topic&& other) = default;
        log_topic(log_topic const& other) = default;

        std::string name;
        level default_level_;
        level trigger_level_;
    };

    bool operator<(log_topic const& lhs, log_topic const& rhs){
        return lhs.name < rhs.name;
    }
    bool operator==(log_topic const& lhs, log_topic const& rhs){
        return lhs.name == rhs.name;
    }


    struct logger {
        logger() = delete;
        logger(logger const&) = delete;
        logger(logger&&) = delete;

        // logging is configured globally via these varialbes
        // configure logging before you start logging!!!
        static log_topic Default;
        static log_topic Network;
        static bool Do_Filename;
        static bool Do_Function;

        static bool is_active(level level_, log_topic topic = logger::Default){
            return level_ <= topic.trigger_level_;
        }

        static std::stringstream create_log_stream(log_topic& topic, level level_,
                                                   const char* file_name, int line_no,
                                                   const char* function){
            std::stringstream ss;
            if(Do_Filename){
                ss << file_name << ":" << line_no;
            }
            if(Do_Function){
                ss << "(" << function << ") - ";
            }
            ss << level_to_str(level_);
            if(!(topic == Default)){
                ss << "(" << topic.name << ")";
            }
            ss << ": ";
            return ss;
        }
    };

    //here we could write to multiple destinations
    struct log_stream_consumer{
        void operator&(std::ostream& ss){ std::cerr << ss.rdbuf() << std::endl; };
    };
}}}
