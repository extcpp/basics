#include <vector>
#include <map>
#include <iostream>
#include <obi/util/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    {
        using namespace obi::util::logging;
        logger::Do_Function = true;
        std::cout << level_to_str(0) << std::endl;
        std::cout << level_to_str(10) << std::endl;
        std::cout << level_to_str(20) << std::endl;
        std::cout << level_to_str(100) << std::endl;
        std::cout << level_to_str(110) << std::endl;
    }

    OBI_LOG_TOPIC(obi::util::logging::level::error
                 ,obi::util::logging::logger::Network) << "topic level";
    OLOG_TOPIC(error, Network) << "topic level";
    OLOG(error) << "level";

    return 0;
}

