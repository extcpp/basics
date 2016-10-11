#pragma once

#include "definitions.hpp"
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace obi { namespace  util { namespace logging {
    namespace _detail {
        inline bool level_is_active(level level_, logtopic topic = topic::no_topic){
            return level_ <= topic.trigger_level;
        }

        inline std::stringstream create_log_stream(logtopic const& topic, level level_,
                                                   const char* file_name, int line_no,
                                                   const char* function){
            std::stringstream ss;
            if(configuration::do_filename){
                ss << file_name << ":" << line_no;
            }
            if(configuration::do_function){
                ss << "(" << function << ") - ";
            }
            ss << level_to_str(level_);
            if(topic.id != topic::no_topic.id){
                ss << "(" << topic.name << ")";
            }
            ss << ": ";
            return ss;
        }

        //here we could write to multiple destinations
        struct log_stream_consumer{
            void operator&(std::ostream& ss){ std::cerr << ss.rdbuf() << std::endl; };
        };
    } // _detail

}}} // obi::util::logging
