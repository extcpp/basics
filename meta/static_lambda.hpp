// taken from http://pfultz2.com/blog/2014/09/02/static-lambda/
#pragma once

#include <type_traits>
#include <utility>

namespace obi { namespace meta {

#define OBI_STATIC_LAMBDA obi::util::wrapper_factor() += true ? nullptr : obi::util::addr_add() + []

struct addr_add
{
    template<class T>
    friend typename std::remove_reference<T>::type* operator+(addr_add, T &&t)
    {
        return &t;
    }
};

template<class F>
struct wrapper
{
    static_assert(std::is_empty<F>(), "Lambdas must be empty");

    template<class... Ts>
    decltype(auto) operator()(Ts&&... xs) const
    {
        return reinterpret_cast<const F&>(*this)(std::forward<Ts>(xs)...);
    }
};

struct wrapper_factor
{
    template<class F>
    constexpr wrapper<F> operator += (F*)
    {
        return {};
    }
};

}}  // namespace obi::meta
