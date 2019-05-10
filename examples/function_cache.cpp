// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <obi/util/function_cache.hpp>
#include <obi/util/scoped_timer.hpp>
#include <iostream>
#include <future>

using namespace std;
using namespace obi::util;

uint64_t fib(uint64_t n) {
    if (n < 2) { return n; }
    future<uint64_t> n1 = async(fib, n - 1);
    future<uint64_t> n2 = async(fib, n - 2);
    return n1.get() + n2.get();
}

int main() {
    scoped_timer timer("function cache test");

    auto fib_cache = add_function_cache(fib);
    std::string result;
    timer.add_step("init - done");

    result = std::to_string(fib(15ull)) ;
    timer.add_step("result: " + result + " - no cache");
    result = std::to_string(fib_cache(15ull)) ;
    timer.add_step("result: " + result + " - build cache");
    result = std::to_string(fib_cache(15ull)) ;
    timer.add_step("result: " + result + " - use cache");

    return 0;
}
