// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/pretty.hpp>
#include <iostream>
#include <map>
#include <vector>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using ext::util::pretty::operator<<;

    // define data
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};

    using Map = std::map<int, std::string>;
    Map map{{2, "foo"}, {3, "bar"}};
    std::vector<Map> vec_of_maps;
    vec_of_maps.push_back(map);
    vec_of_maps.push_back(map);

    std::vector<std::vector<std::string>> vec2 = {{"foo", "bar"}, {"barz"}};

    // cool stuff starts below

    // do this when you really need the string
    std::cout << ext::util::pretty::fmt(vec) << std::endl;

    // prefer this!
    std::cout << vec << std::endl;

#ifndef CLANG_ANALYZE
    // a bit more magic
    std::cout << vec_of_maps << std::endl;

    // prefer this!
    std::cout << vec2 << std::endl;
#endif

    return 0;
}
