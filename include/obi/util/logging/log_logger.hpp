#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "log_definitions.hpp"
#include <algorithm>
#include <sstream>

namespace obi::util::logging {
    struct log_topic {
        log_topic(std::string&& n, log_level def, log_level trig) :
            name(std::move(n)), default_level(def), trigger_level(trig){}
        log_topic(log_topic&& other) = default;
        log_topic(log_topic const& other) = default;

        std::string name;
        log_level default_level;
        log_level trigger_level;
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

        static log_topic Default;
        static log_topic Network;
		static bool do_filename;
		static bool do_function;

        static bool is_active(log_level level, log_topic topic = logger::Default){
            return level <= topic.trigger_level;
        }

        static std::stringstream create_log_stream(log_topic& topic, log_level level,
												   const char* file_name, int line_no,
												   const char* function){
			std::stringstream ss;
			if(do_filename){
				ss << file_name << ":" << line_no;
			}
			if(do_function){
				ss << "(" << function << ") - ";
			}
			ss << level_to_str(level);
			if(!(topic == Default)){
				ss << "(" << topic.name << ")";
			}
			ss << ": ";
			return ss;
        }
    };

	struct log_stream_consumer{
		log_stream_consumer(){};
	};

	std::ostream& operator<<(std::ostream& lhs, std::stringstream&& rhs){
		lhs << rhs.rdbuf();
		return lhs;
	}

	struct stream_to_void{
		void operator&(log_stream_consumer&){};
		void operator&(std::ostream& os){ os << std::endl; };
	};
}
