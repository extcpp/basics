// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/macros/platform.hpp>
#ifdef EXT_WINDOWS
int main() { }
#else

#include <iostream>
#include <ext/util/functional.hpp>

using namespace std;
using namespace ext::util;

int main() {
    auto rv1 = apply(add)(11, 12);

    string hello("hello functional ");
    string world("world!");
    auto rv2 = apply(add)(hello, world);

    cout << curry(add,1)(1) << endl;

// FIXME
#ifndef _WIN32
    cout << rv1 << endl
         << rv2 << endl;
#endif // _WIN32

    return 0;
}
#endif // EXT_WINDOWS
