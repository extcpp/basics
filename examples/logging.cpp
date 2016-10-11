#include <vector>
#include <map>
#include <iostream>
#include <obi/util/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    using namespace obi::util::logging;
    configuration::do_function = true;
    std::cout << _detail::level_to_str(level::fatal) << std::endl;
    std::cout << _detail::level_to_str(level::trace) << std::endl;

    OBI_LOG_TOPIC(obi::util::logging::level::error
                 ,obi::util::logging::topic::network) << "error network";

    configuration::global_level = level::fatal;
    OLOG_TOPIC(error, network) << "error network";
    OLOG(warn) << "warning";
    OLOG(error) << "error";
    OLOG_TOPIC(info, network) << "info network";

    using namespace obi::util::logging;
    configuration::global_level = level::trace;
    OLOG_TOPIC(trace, network) << "tace network";

    return 0;
}

