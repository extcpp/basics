// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

// Inspired by Daniel Frey's awesome talk about non recusive
// template programming at the c++ user group Aachen
// https://github.com/taocpp/tuple/blob/master/Variadic%20Templates.pdf

#include <type_traits>
#include <tuple>
namespace obi { namespace meta {

// tuple wrapping
template<typename>
struct tuple_wrapper {};

// wrap
template<typename>
struct tuple_wrap {};

template<typename... XS>
struct tuple_wrap<std::tuple<XS...>>{
    using type = std::tuple<tuple_wrapper<XS>...>;
};

template<typename T> using tuple_wrap_t = typename tuple_wrap<T>::type;

// unwrap
template<typename>
struct tuple_unwrap {};

template<typename... XS>
struct tuple_unwrap<std::tuple<tuple_wrapper<XS>...>>{
    using type = std::tuple<XS...>;
};

template<typename T> using tuple_unwrap_t = typename tuple_unwrap<T>::type;

template<typename ...XS>
using tuple_cat_t = tuple_unwrap_t<
                        decltype(
                            std::tuple_cat(
                                std::declval<tuple_wrap_t<XS>>()...)
                        )
                    >;
// tuple wrapping - end

// tuple size
template<typename>
struct tuple_size;

template<typename ...XS>
struct tuple_size<std::tuple<XS...>> : std::integral_constant<std::size_t, sizeof...(XS)>
{};

template <typename ...XS> constexpr std::size_t tuple_size_v = tuple_size<XS...>::value;
// tuple size - end


// replace in tuple
template<typename T , typename R , typename ...XS>
using replace_in_tuple_t =
    std::tuple<std::conditional_t<std::is_same<XS, T>::value
                                 ,R
                                 ,XS
                                 > ...
              >;
// replace in tuple - end

}}  // namespace obi::meta
