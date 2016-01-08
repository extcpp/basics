// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once

#include <type_traits>
#include "void_t.hpp"

namespace obi { namespace meta {

// has_value_var
    template<typename T, typename = void>
    struct has_value_var : std::false_type {};

    template<typename T>
    struct has_value_var<T, void_t<decltype(std::declval<T>().value)>> : std::true_type {};
    //struct has_value_var<T, decltype(std::declval<T>().value, void())> : std::true_type {};  // warning unused var

// has_type_member
    template<class, class = void>
    struct has_type_member : std::false_type {};

    template<class T>
    struct has_type_member<T, void_t<typename T::type> > : std::true_type {};
    //struct has_type_member<T, decltype(typename T::type(),void())> : std::true_type {};

    //check for category type
    template<class, class = void>
    struct has_category_member : std::false_type {};

    template<class T>
    struct has_category_member<T, ::obi::meta::void_t<typename T::category> > : std::true_type {};

    template <class T, class = void>
    struct is_input_iterator : std::false_type  {};

    template <class T>
    struct is_input_iterator<T, void_t< decltype(++std::declval<T&>())                      //increment
                                      , decltype(*std::declval<T&>())                       //dereference
                                      , decltype(std::declval<T&>() == std::declval<T&>())  //compare
                                      >
                            > : std::true_type {};

}}  // namespace obi::meta
