#include <obi/macros/compiler.hpp>
#include <obi/util/logging.hpp>
OBI_INIT_PRIORITY_VC_HIGH

namespace obi { namespace  util { namespace logging {
    OBI_INIT_PRIORITY_GNU(101) std::mutex _detail::logmutex{};
    OBI_INIT_PRIORITY_GNU(102) std::map<int,_detail::logtopic*> _detail::topics_map{};
    OBI_INIT_PRIORITY_GNU(103) _detail::logtopic topic::no_topic {1, std::string("default"), level::error};
    OBI_INIT_PRIORITY_GNU(103) _detail::logtopic topic::network {2, std::string("network"), level::error};
    OBI_INIT_PRIORITY_GNU(103) _detail::logtopic topic::engine {3, std::string("engine"), level::error};
    bool configuration::threads{false};
    bool configuration::filename{true};
    bool configuration::function{true};
    bool configuration::vim{true};

    _detail::logger::logger(logtopic const& topic, level level_,
                            const char* file_name, int line_no,
                            const char* function):ss() {
        ss << "\n";
        if (configuration::vim){
            ss << "vim " << file_name << " +" << line_no << "\n";
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

    _detail::logger::~logger(){
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
}}} // obi::util::logging
