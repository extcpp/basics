// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/container_helper.hpp>
#include <ext/util/show.hpp>
#include <iostream>
#include <numeric>
#include <ostream>

int main() {
    using ext::util::operator<<;
    namespace eu = ext::util;
    std::vector<int> vec(7);

    iota(vec.begin(), vec.end(), 1);
    std::list<int> list(vec.begin(), vec.end());
    std::set<int> set(vec.begin(), vec.end());


    std::cout << vec << "\n";

    std::cout << "-- vector erase 3 --\n";
    eu::erase(vec, 3);
    std::cout << vec << "\n";

    std::cout << "-- vector erase % 2 --\n";
    eu::erase_if(vec, [](int n) {
        return n % 2 == 0;
    });
    std::cout << vec << "\n";


    std::cout << "-- list erase 3 --\n";
    eu::erase(list, 3);
    std::cout << list << "\n";

    std::cout << "-- list erase % 2 --\n";
    eu::erase_if(list, [](int n) {
        return n % 2 == 0;
    });
    std::cout << list << "\n";

    std::cout << "-- set erase 2 --\n";
    eu::erase(set, 2);
    std::cout << set << "\n";

    std::cout << "-- set erase 4 > x < 7 --\n";
    eu::erase_if(set, [](int n) {
        return n > 4 && n < 7;
    });
    std::cout << set << "\n";
}
