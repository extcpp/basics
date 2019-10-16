// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <iostream>
#include <map>
#include <vector>

#ifndef _WIN32
    #include <ext/logging.hpp>

int main(/*int argc, const char *argv[]*/) {
    {
        using namespace ext::logging;
        configuration::function = false;
    }

    EXT_DEV << "geil";
    EXT_DEVV << "ballern";

    EXT_LOG("cafe") << "default level and topic";
    EXT_LOG("0000", trace) << "where is the byte gone";
    EXT_LOG("1111", network, debug) << "ohlala";
    EXT_LOG("2222", info) << "Hi there!";
    EXT_LOG("3333", warn) << "something is wrong";
    EXT_LOG("4444", network, error, false) << "your network is broken";
    EXT_LOG("5555", network, error, true) << "your network is broken";
    EXT_LOG("6666", fatal) << "your app will terminate";

#else
int main(/*int argc, const char *argv[]*/) {
#endif // _WIN32
    return 0;
}
