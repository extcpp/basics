// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/macros/platform.hpp>
#if defined(EXT_WINDOWS) || defined(EXT_MACOS)
int main() {}
#else

    #include <ext/util/functional.hpp>
    #include <iostream>

using namespace std;
using namespace ext::util;

int main() {
    auto rv1 = apply(add)(11, 12);

    string hello("hello functional ");
    string world("world!");
    auto rv2 = apply(add)(hello, world);

    cout << curry(add, 1)(1) << endl;

    // FIXME
    #ifndef _WIN32
    cout << rv1 << endl << rv2 << endl;
    #endif // _WIN32

    return 0;
}
#endif     // defined(EXT_WINDOWS) || defined(EXT_MACOS)
