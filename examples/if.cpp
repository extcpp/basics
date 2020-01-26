// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/meta/if.hpp>
#include <iostream>

using namespace std;
using namespace ext::meta;

int main() {
    cout << "\nnext\n"
         << if_t<std::true_type, double, int>(1.5) << "\nnext\n"
         << if_t<std::false_type, double, int>(1.5) << "\nnext\n"
         << if_c_t<true, double, int>(1.5) << "\nnext\n"
         << if_c_t<false, double, int>(1.5) << endl;

    return 0;
}
