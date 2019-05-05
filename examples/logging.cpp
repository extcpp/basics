// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <vector>
#include <map>
#include <iostream>

#ifndef _WIN32
#include <obi/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    {
        using namespace obi::logging;
        configuration::function = false;
    }

    OBI_DEV  << "geil";
    OBI_DEVV << "ballern";

    OBI_LOG("cafe") << "default level and topic";
    OBI_LOG("0000", trace) << "where is the byte gone";
    OBI_LOG("1111", network, debug) << "ohlala";
    OBI_LOG("2222", info) << "Hi there!";
    OBI_LOG("3333", warn) << "something is wrong";
    OBI_LOG("4444", network, error, false) << "your network is broken";
    OBI_LOG("5555", network, error, true) << "your network is broken";
    OBI_LOG("6666", fatal) << "your app will terminate";

#else
int main(/*int argc, const char *argv[]*/) {
#endif // _WIN32
    return 0;
}

