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
}}}
