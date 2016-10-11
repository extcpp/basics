#include <obi/util/logging.hpp>

namespace obi { namespace  util { namespace logging {
    std::map<int,_detail::logtopic*> _detail::topics_map = {};
    _detail::logtopic topic::no_topic = _detail::logtopic{1, std::string("default"), level::error};
    _detail::logtopic topic::network  = _detail::logtopic{2, std::string("network"), level::error};
    _detail::logtopic topic::engine   = _detail::logtopic{3, std::string("engine"), level::error};
    bool configuration::do_filename = false;
    bool configuration::do_function = false;
}}}
