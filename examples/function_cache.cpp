#include <obi/util/function_cache.hpp>
#include <obi/util/scoped_timer.hpp>
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

    //function_cache - usage
    //auto fib_cache = add_function_cache(&fib);

    //function_cache_old - usage
    auto fib_cache_old = add_function_cache_old(function<decltype(fib)>(fib));

    timer.add_step("init - done");

    //cout << fib_cache(15ull) << endl;
    timer.add_step("function_cache (without cache) - done");
    //cout << fib_cache(15ull) << endl;
    timer.add_step("function_cache (with cache) - done");

    cout << fib_cache_old(15ull) << endl;
    timer.add_step("function_cache_old (without cache) - done");
    cout << fib_cache_old(15ull) << endl;
    timer.add_step("function_cache_old (with cache) - done");

    return 0;
}
