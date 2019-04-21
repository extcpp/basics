// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_FUNCTION_CACHE_HEADER
#define OBI_UTIL_FUNCTION_CACHE_HEADER

#include <functional>
#include <type_traits>
#include <map>
#include <tuple>

namespace obi { namespace util {

// requires c++14
// Function Pointer only
//
// TODO - add parameter that limits the size of the cache
//        this is difficult because we can have data of
//        different functions in the same map and we are
//        only able to distinguish by function pointer
//        value
//
auto add_function_cache = [](auto fun, unsigned cache_size=1000000, double delete_percent=0.1) {
    using fun_type = decltype(fun);
    return ([=](auto... run_args) {
        using fun_return_type = std::result_of_t<fun_type(decltype(run_args)...)>;
        static unsigned delete_on_overflow = static_cast<unsigned>(cache_size * delete_percent);
        static std::map<
            std::tuple<fun_type, decltype(run_args)...>,
            fun_return_type
        > result_cache;
        static std::map<fun_type, unsigned> inserts;
        std::tuple<fun_type, decltype(run_args)...> tuple(fun, run_args...);
        auto result_search = result_cache.find(tuple);
        if (result_search == result_cache.end()){
            auto insert_num_search = inserts.find(fun);
            if (insert_num_search == inserts.end()){
                inserts[fun]=0;
            } else {
                insert_num_search->second++;
                // TODO no overflow!!
                // in case of overflow delete
            }
            (void) delete_on_overflow; //silence warning
            fun_return_type rv = fun(run_args...);
            result_cache[tuple] = rv;
            return rv;
        }
        else {
            return result_search->second;
        }
    });
};

template <typename R, class... Args> auto
add_function_cache_old(std::function<R(Args...)> fun)
-> std::function<R(Args...)>
{
    std::map<std::tuple<Args...>, R> cache;
    return [=](Args... args) mutable {
        std::tuple<Args...> t(args...);
        auto search = cache.find(t);
        if (search == cache.end()) {
            R rv = fun(args...);
            cache[t] = rv;
            return rv;
        }
        else {
            return search->second;
        }
    };
}

}}  // namespace obi::util
#endif // OBI_UTIL_FUNCTION_CACHE_HEADER