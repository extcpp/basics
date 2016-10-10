#include <obi/util/logging.hpp>

namespace obi { namespace  util { namespace logging {
    log_topic logger::Default = log_topic{"default",  level::error, level::error};
    log_topic logger::Network = log_topic{"network",  level::info, level::error};
	bool logger::Do_Filename = false;
	bool logger::Do_Function = false;
}}}
