// Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_FUNCTION_CACHE_HEADER
#define OBI_UTIL_FUNCTION_CACHE_HEADER

#include <functional>
#include <type_traits>
#include <map>
#include <tuple>

namespace obi { namespace util {

// do not use this for cheap operations :)
auto add_function_cache = [](auto fun, std::size_t cache_size=10000) {
    return ([=](auto... run_args) -> decltype(fun(run_args...)) {
        using fun_type = decltype(fun);
        using fun_return_type = decltype(fun(run_args...));
        using function_arg_tuple = std::tuple<fun_type, decltype(run_args)...>;
        using result_cache = std::map<function_arg_tuple, std::pair<fun_return_type,std::size_t>>;

        static result_cache cache;
        static std::map<fun_type, std::size_t> inserts;

        // search result in cache
        function_arg_tuple tuple(fun, run_args...);
        auto result_search = cache.find(tuple);

        // handle insert or return value
        if (result_search == cache.end()){
            auto insert_num_search = inserts.find(fun);
            std::size_t insert_id = 0;
            if (insert_num_search == inserts.end()){
                inserts[fun]=0;
            } else {
                insert_id = insert_num_search->second++;
                if (insert_id %  cache_size / 10){
                    // get elements for this function
                    std::size_t elements_for_fun = 0;
                    if (elements_for_fun > cache_size) {
                        // clean up - delete cached elements
                    }
                }
            }
            fun_return_type rv = fun(run_args...);
            cache[tuple] = {rv, insert_id};
            return rv;
        } else {
            return result_search->second.first;
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
