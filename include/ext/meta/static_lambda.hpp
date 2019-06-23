// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
// taken from http://pfultz2.com/blog/2014/09/02/static-lambda/
#pragma once
#ifndef EXT_META_STATIC_LAMBDA_HEADER
#    define EXT_META_STATIC_LAMBDA_HEADER

#    include <type_traits>
#    include <utility>

namespace ext { namespace meta {

#    define EXT_STATIC_LAMBDA ext::meta::wrapper_factor() += true ? nullptr : ext::meta::addr_add() + []

struct addr_add {
    template<class T>
    friend typename std::remove_reference<T>::type* operator+(addr_add, T&& t) {
        return &t;
    }
};

template<class Functor>
struct wrapper {
    static_assert(std::is_empty<Functor>(), "Lambdas must be empty");

    template<class... XXS>
    decltype(auto) operator()(XXS&&... xxs) const {
        return reinterpret_cast<const Functor&>(*this)(std::forward<XXS>(xxs)...);
    }
};

struct wrapper_factor {
    template<class Functor>
    constexpr wrapper<Functor> operator+=(Functor*) {
        return {};
    }
};
}}     // namespace ext::meta
#endif // EXT_META_STATIC_LAMBDA_HEADER
