#include <vector>
#include <map>
#include <iostream>
#include <obi/util/show.hpp>

int main(int argc, const char *argv[]) {
    // pull in operator
    using obi::util::operator<<;

    // define data
    std::vector<int> vec = {1,2,3,4,5,6};

    using Map = std::map <int, std::string>;
    Map map {{ 45, "foo" },{ 3, "bar" }};
    std::vector<Map> vec_of_maps;
    vec_of_maps.push_back(map);
    vec_of_maps.push_back(map);


    // cool stuff starts below


    // do this when you really need the sting
    std::cout << obi::util::show(vec) << std::endl;

    //prefer this!
    std::cout << vec << std::endl;

    //a bit more magic
    std::cout << vec_of_maps << std::endl;

    return 0;
}

