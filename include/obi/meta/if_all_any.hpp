// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <type_traits>
namespace obi { namespace meta {

// REQUIRES GCC 8.0

// if_all  - logical and (fold expression)
template<bool ...XS>
using if_all = std::bool_constant<(XS && ...)>;

template<bool ...XS>
using if_all_t = typename if_all<XS...>::type;

template<bool ...XS>
constexpr bool if_all_v = if_all<XS...>::value;


// if_any - logical or (fold expression)
template<bool ...XS>
//using if_any = std::bool_constant<(XS || ...)>;
using if_any = std::bool_constant<!if_all<!XS...>::value>; // share types

template<bool ...XS>
using if_any_t = typename if_any<XS...>::type;

template<bool ...XS>
constexpr bool if_any_v = if_any<XS...>::value;


// enable if helper
template<typename T, bool ...XS>
using enable_if_all_t = typename std::enable_if<(XS && ...),T>::type;

template<typename T, bool ...XS>
using enable_if_any_t = typename std::enable_if<(XS || ...),T>::type;


}}  // namespace obi::meta
