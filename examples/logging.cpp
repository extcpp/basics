#include <vector>
#include <map>
#include <iostream>
#include <obi/util/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    {
        using namespace obi::util::logging;
        configuration::function = false;
    }

    OBI_LOG_TOPIC(error, network) << "packages lost";
    OBI_LOG(fatal) << "General Excoption";

    return 0;
}

