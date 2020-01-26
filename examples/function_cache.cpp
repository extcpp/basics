// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/macros/platform.hpp>
#if defined(EXT_MACOS)
int main() {}
#else

// clang-format off
#include <ext/util/function_cache.hpp>
#include <ext/util/scoped_timer.hpp>
#include <future>
#include <iostream>
#include <string>
// clang-format on

using namespace std;
using namespace ext::util;
using namespace std::literals::string_literals;

uint64_t fib(uint64_t n) {
    if (n < 2) {
        return n;
    }
    future<uint64_t> n1 = async(fib, n - 1);
    future<uint64_t> n2 = async(fib, n - 2);
    return n1.get() + n2.get();
}

int main() {
    scoped_timer timer("function cache test");

    auto fib_cache = add_function_cache(fib);
    std::string result;
    timer.add_step("init - done");

    result = std::to_string(fib(15ull));
    timer.add_step("result: "s + result + " - no cache"s);
    result = std::to_string(fib_cache(15ull));
    timer.add_step("result: "s + result + " - build cache"s);
    result = std::to_string(fib_cache(15ull));
    timer.add_step("result: "s + result + " - use cache"s);

    return 0;
}
#endif // defined(EXT_MACOS)
