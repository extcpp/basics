#include <iostream>
#include <libobi/util/functional.hpp>

using namespace std;
using namespace obi::util;

int main() {
    auto rv1 = apply(add)(11, 12);

    string hello("hello functional ");
    string world("world!");
    auto rv2 = apply(add)(hello, world);

    cout << curry(add,1)(1) << endl;

    cout << rv1 << endl
         << rv2 << endl;

    return 0;
}
