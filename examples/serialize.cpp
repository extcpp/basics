// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <obi/util/serialization.hpp>

int main(/*int argc, const char *argv[]*/) {
    std::uint64_t a_in = 42;
    auto volatile array = obi::util::to_big_storage(a_in);
    (void) array;
    return 0;
}

