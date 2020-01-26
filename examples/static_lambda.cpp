// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/meta/static_lambda.hpp>
#include <iostream>

using namespace std;
using namespace ext::meta;

int main() {
    const constexpr auto add_one = EXT_STATIC_LAMBDA(auto x) {
        return x + 1;
    };

    std::cout << "1 + 1 = " << add_one(1) << endl;
}
