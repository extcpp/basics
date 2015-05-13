// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once

#include <type_traits>

namespace obi { namespace meta {

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
    using is_one_of_t = typename is_one_of<T, XXS...>::type;

}}  // namespace obi::meta
