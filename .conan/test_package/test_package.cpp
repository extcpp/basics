// Copyright (c) 2020 Jan Christoph Uhde
// Please see LICENSE for license or visit https://github.com/extcpp/libext

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <ext/util/show.hpp>

int main()
{
    std::vector<std::string> out{"this", "lib", "compiles"};
    using ext::util::operator<<;
    std::cout << out << std::endl;
    return EXIT_SUCCESS;
}
