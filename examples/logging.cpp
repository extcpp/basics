#include <vector>
#include <map>
#include <iostream>
#include <obi/util/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    using namespace obi::util::logging;
	logger::do_function = true;
    std::cout << level_to_str(0) << std::endl;
    std::cout << level_to_str(10) << std::endl;
    std::cout << level_to_str(20) << std::endl;
    std::cout << level_to_str(100) << std::endl;
    std::cout << level_to_str(110) << std::endl;

	OBI_TOPICLOG(logger::Network,error) << "topic level";
	OBI_LOG(error) << "level";

    return 0;
}

