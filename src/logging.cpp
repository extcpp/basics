#include <obi/util/logging.hpp>

namespace obi { namespace  util { namespace logging {
    level configuration::global_level = level::error;
    _detail::logtopic topic::unknown = _detail::logtopic{"default", level::error};
    _detail::logtopic topic::network = _detail::logtopic{"network", level::error};
    bool configuration::do_filename = false;
    bool configuration::do_function = false;
}}}
