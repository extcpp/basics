// taken from http://pfultz2.com/blog/2014/09/02/static-lambda/
#pragma once

#include <type_traits>
#include <utility>

namespace obi { namespace meta {

#define OBI_STATIC_LAMBDA obi::meta::wrapper_factor() += true ? nullptr : obi::meta::addr_add() + []

struct addr_add
{
    template<class T>
    friend typename std::remove_reference<T>::type* operator+(addr_add, T &&t)
    {
        return &t;
    }
};

template<class Functor>
struct wrapper
{
    static_assert(std::is_empty<Functor>(), "Lambdas must be empty");

    template<class... XXS>
    decltype(auto) operator()(XXS&&... xxs) const
    {
        return reinterpret_cast<const Functor&>(*this)(std::forward<XXS>(xxs)...);
    }
};

struct wrapper_factor
{
    template<class Functor>
    constexpr wrapper<Functor> operator += (Functor*)
    {
        return {};
    }
};

}}  // namespace obi::meta
