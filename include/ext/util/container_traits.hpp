// Copyright - 2016-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_CONTAINER_TRAITS_HEADER
#define EXT_UTIL_CONTAINER_TRAITS_HEADER
#include <ext/meta/basic.hpp>
#include <ext/util/forward_std_string.hpp>

#include <string_view>

namespace ext { namespace util { namespace _detail {

template<typename T, typename = void>
struct has_iterator : std::false_type {};

template<typename T>
struct has_iterator<T,
                    std::void_t<typename T::iterator,
                                typename T::const_iterator,
                                decltype(std::declval<T&&>().begin()),
                                decltype(std::declval<T&&>().end())>> : std::true_type {};

template<typename T, class... TT>
struct is_excluded
    : std::bool_constant<(std::is_same<meta::remove_cv_ref_t<T>, meta::remove_cv_ref_t<TT>>::value || ...)> {};

template<typename T>
struct is_container
    : std::bool_constant<has_iterator<meta::remove_cv_ref_t<T>>::value &&
                         !is_excluded<T, std::string, std::string_view>::value> {};

template<typename T, typename = void>
struct is_associative : std::false_type {};

template<typename T>
struct is_associative<
    T,
    std::void_t<typename meta::remove_cv_ref_t<T>::key_type, typename meta::remove_cv_ref_t<T>::mapped_type>>
    : std::true_type {};

template<typename T, typename = void>
struct is_set : std::false_type {};

template<typename T>
struct is_set<T, std::void_t<typename meta::remove_cv_ref_t<T>::value_compare>> : std::true_type {};

}}} // namespace ext::util::_detail

#endif // EXT_UTIL_CONTAINER_TRAITS_HEADER
