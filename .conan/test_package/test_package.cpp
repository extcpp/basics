// Copyright (c) 2020 Jan Christoph Uhde
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <ext/util/show.hpp>

int main()
{
    std::vector<std::string> out{"ext::basics", "successfully", "installs"};
    using ext::util::operator<<;
    std::cout << "\n\n" << out << "\n" << std::endl;
    return EXIT_SUCCESS;
}
