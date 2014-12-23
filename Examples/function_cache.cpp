#include <libobi/util/function_cache.hpp>
#include <libobi/util/scoped_timer.hpp>
#include <iostream>
#include <future>

using namespace std;
using namespace obi::util;


uint64_t fib(uint64_t n)
{
    if (n < 2)
        return n;

    future<uint64_t> n1 = async(fib, n - 1);
    future<uint64_t> n2 = async(fib, n - 2);

    return n1.get() + n2.get();
}

int main() {

    scoped_timer timer("function cache test");
    function<decltype(fib)> fib_fn = &fib;
    auto fib_cache = add_function_cache(fib_fn);

    timer.add_step("init - done");
    cout << fib_cache(8) << endl;
    timer.add_step("without cache - done");
    cout << fib_cache(8) << endl;
    timer.add_step("with cache - done");

    return 0;
}
