// Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_FUNCTION_CACHE_HEADER
#define EXT_UTIL_FUNCTION_CACHE_HEADER
#include <functional>
#include <map>
#include <tuple>
#include <type_traits>

namespace ext { namespace util {

namespace _detail {
template<typename R, class... Args>
auto add_function_cache_impl(std::function<R(Args...)> fun) -> std::function<R(Args...)> {
    std::map<std::tuple<Args...>, R> cache;
    return [=](Args... args) mutable {
        std::tuple<Args...> t(args...);
        auto search = cache.find(t);
        if (search == cache.end()) {
            R rv = fun(args...);
            cache[t] = rv;
            return rv;
        } else {
            return search->second;
        }
    };
}
} // namespace _detail

template<typename T>
auto add_function_cache(T&& x) {
    return _detail::add_function_cache_impl(std::function(std::forward<T>(x)));
}
}}     // namespace ext::util
#endif // EXT_UTIL_FUNCTION_CACHE_HEADER
