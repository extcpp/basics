#include <libobi/util/function_cache.hpp>
#include <iostream>

using namespace std;
using namespace obi::util;

int foo(int a, int b){
    return a+b;
}

int main() {
    std::function<decltype(foo)> foo_fn = &foo;
    auto foo_cache = add_function_cache(foo_fn);

    cout << foo_cache(1,2) << endl;
    cout << foo_cache(1,2) << endl;

    return 0;
}
