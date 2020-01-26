// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/structures.hpp>
#include <iostream>

using namespace ext::structures;

int main(int argc, const char* argv[]) {
    multi_array<int, 5, 3, 3> a;

    // a.at(1,2,3);
    std::cout << a.size() << std::endl;

    return 0;
}
