// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_META_TUPLE_HEADER
#define OBI_META_TUPLE_HEADER

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

// for each in tuple
namespace _detail {
    template <std::size_t... index, typename Tuple, typename Functor, typename... FArgs>
    void tuple_apply_helper(std::index_sequence<index...>, Tuple&& t, Functor&& f, FArgs&& ...fargs){
        using discard = int[];
        (void) discard{0, (void(f(std::get<index>(t),fargs...)),0) ...};
    }
}

template <typename Tuple, typename Functor, typename... FArgs>
void tuple_apply(Tuple&& t, Functor&& f, FArgs&& ...fargs){
    constexpr std::size_t size= std::tuple_size<std::decay_t<Tuple>>::value;
    _detail::tuple_apply_helper(std::make_index_sequence<size>{}
                               ,std::forward<Tuple>(t)
                               ,std::forward<Functor>(f)
                               ,std::forward<FArgs>(fargs)...
                               );
}
// for each in tuple - end

}}  // namespace obi::meta
#endif // OBI_META_TUPLE_HEADER
