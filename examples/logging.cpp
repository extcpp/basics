// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <vector>
#include <map>
#include <iostream>

#ifndef _WIN32
#include <obi/util/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    {
        using namespace obi::util::logging;
        configuration::function = false;
    }

    OBI_LOG(trace) << "where is the byte gone";
    OBI_LOG(debug) << "ohlala";
    OBI_LOG(info) << "Hi there!";
    OBI_LOG(warn) << "something is wrong";
    OBI_LOG_TOPIC(network, error) << "your network is broken";
    OBI_LOG(fatal) << "your app will terminate";

#else
int main(/*int argc, const char *argv[]*/) {
#endif // _WIN32
    return 0;
}

