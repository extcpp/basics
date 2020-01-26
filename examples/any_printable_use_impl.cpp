// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/any_printable.hpp>
using namespace ext::util;

void use(any_printable const& any) {
    std::cout << any << std::endl;
}
