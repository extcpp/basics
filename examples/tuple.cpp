// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/meta/tuple.hpp>
#include <ext/util/show.hpp>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using ext::util::operator<<;

    // define data
    using combined = ext::meta::tuple_cat_t<std::tuple<int, float, void>, std::tuple<char, double>>;

    std::cout << ext::meta::tuple_size<combined>::value << std::endl;

    std::tuple<int, int, int> tuple1 = {1, 2, 3};
    auto print = [](auto a, int b, int c) {
        std::cout << a << " " << b << " " << c << " " << std::endl;
    };
    ext::meta::tuple_apply(tuple1, print, 23, 42);

    return 0;
}
