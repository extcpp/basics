#include <obi/util/logging.hpp>

namespace obi::util::logging {
    log_topic logger::Default = log_topic{"default",  log_level::error, log_level::error};
    log_topic logger::Network = log_topic{"network",  log_level::info, log_level::error};
	bool logger::do_filename = false;
	bool logger::do_function = false;
}
