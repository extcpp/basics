// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/show.hpp>
#include <iostream>
#include <map>
#include <vector>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using ext::util::operator<<;

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
    std::cout << ext::util::show(vec) << std::endl;

    // prefer this!
    std::cout << vec << std::endl;

    // a bit more magic
    std::cout << vec_of_maps << std::endl;

    // prefer this!
    std::cout << vec2 << std::endl;

    return 0;
}
