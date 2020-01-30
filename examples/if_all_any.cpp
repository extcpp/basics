// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/meta/basic.hpp>
#include <ext/util/show.hpp>
#include <iostream>
#include <map>
#include <vector>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using ext::util::operator<<;

    std::cout << std::boolalpha << ext::meta::if_all_v<true, true> << std::endl
              << ext::meta::if_all_v<true, true> << std::endl
              << ext::meta::if_all_v<true, true, false> << std::endl

              << ext::meta::if_any_v<true, true> << std::endl
              << ext::meta::if_any_v<false, false> << std::endl
              << ext::meta::if_any_v<true, false, true> << std::endl

              << std::endl;
    return 0;
}
