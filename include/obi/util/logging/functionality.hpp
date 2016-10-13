#pragma once
#ifndef OBI_UTIL_LOGGING_FUNCTIONALITY_HPP
#define OBI_UTIL_LOGGING_FUNCTIONALITY_HPP

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
            std::stringstream ss;
            logger(logtopic const& topic, level level_,
                   const char* file_name, int line_no,
                   const char* function = "none"):ss() {
                ss << "\n";
                if (configuration::vim){
                    ss << file_name << " +" << line_no << "\n";
                }
                ss << level_to_str(level_);
                if(topic.id != topic::no_topic.id){
                    ss << "(" << topic.name << ")";
                }
                if(configuration::filename){
                    ss << " - " << basename(file_name) << ":" << line_no;
                }
                if(configuration::function){
                    ss << " - " << function << "()";
                }
                ss << ": ";
            }

            ~logger(){
                if(configuration::threads){
                    // add pointer to global lockfree queue
                    // so other thread releases and prints
                    // free memory in same thread?!
                    // _detail::add_queue(ss.str());
                }
                else{
                    std::lock_guard<std::mutex> lock(logmutex);
                    std::cerr << ss.rdbuf() << std::endl;
                }
            }

            template<typename T>
            void operator<< (T&& value){
                ss << std::forward<T>(value);
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
#endif // OBI_UTIL_LOGGING_FUNCTIONALITY_HPP
