// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once
#ifndef EXT_META_IS_ONE_OF_HEADER
#define EXT_META_IS_ONE_OF_HEADER

#include <type_traits>

namespace ext { namespace meta {

// is_one_of
    template<class T, class ...XXS>
    struct is_one_of;

    template<class T>
    struct is_one_of<T> : std::false_type {};

    template<class T, class ...XS>
    struct is_one_of<T, T, XS...> : std::true_type {};

    template<class T, class X, class ...XS>
    struct is_one_of<T, X, XS...> : is_one_of<T, XS...> {};

    //alias
    template<class T, class ...XXS>
    inline constexpr bool is_one_of_v = is_one_of<T, XXS...>::value;

}}  // namespace ext::meta
#endif // EXT_META_IS_ONE_OF_HEADER
