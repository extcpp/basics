#pragma once

#include <functional>
#include <map>
#include <tuple>

namespace obi { namespace util {

template <typename R, class... Args> auto
add_function_cache(std::function<R(Args...)> fun)
-> std::function<R(Args...)>
{
    std::map<std::tuple<Args...>, R> cache;
    return [=](Args... args) mutable  {
        std::tuple<Args...> t(args...);
        if (cache.find(t) == cache.end()) {
            R rv = fun(args...);
            cache[t] = rv;
            return rv;
        }
        else {
            return cache[t];
        }
    };
};


/* //TODO fix this -- requires c++14
auto add_function_cache_first_shot = [](auto fun) {
    return ([=](auto... run_args){
        static std::map<
            std::tuple<decltype(run_args)...>,
            decltype(fun)
        > result_cache;

        std::tuple<decltype(run_args)...> tuple(run_args...);
        if (result_cache.find(tuple) == result_cache.end()) {
            auto rv = function(run_args);
            result_cache[tuple] = fun(run_args);
            return rv;
        }
        else {
            return result_cache[tuple];
        }
        return function(run_args);
    });
}; */

}}  // namespace obi::util
