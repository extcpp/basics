// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <obi/structures.hpp>
#include <iostream>

using namespace obi::structures;

int main(int argc, const char *argv[])
{
    multi_array<int,5,3,3> a;

    //a.at(1,2,3);
    std::cout << a.size() << std::endl;

    return 0;
}
