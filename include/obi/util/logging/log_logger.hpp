#pragma once

#include "log_definitions.hpp"
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace obi { namespace  util { namespace logging {
    namespace configuration {
        extern level global_level;
        // logging is configured globally via these varialbes
        // configure logging before you start logging!!!
        extern bool do_filename;
        extern bool do_function;
    }

    namespace topic{
        extern _detail::logtopic unknown;
        extern _detail::logtopic network;
    }

    namespace _detail {
        inline bool level_is_active(level level_, logtopic topic = topic::unknown){
            //int l  = (int) level_;
            //int g  = (int) configuration::global_level;
            //int tt = (int) topic.trigger_level;
            //int r  = (int) (configuration::global_level > topic.trigger_level
            //               ? configuration::global_level : topic.trigger_level );

            //std::cout <<"l:" << l << " "
            //          <<"g:" << g <<" "
            //          <<"t:" << tt << " "
            //          <<"r:" << r << " "
            //          << std::endl;

            return level_ <= (configuration::global_level > topic.trigger_level
                   ? configuration::global_level : topic.trigger_level );

        }

        inline std::stringstream create_log_stream(logtopic& topic, level level_,
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
            if(!(topic == topic::unknown)){
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
