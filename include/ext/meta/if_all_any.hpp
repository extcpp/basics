// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_META_IF_ALL_ANY_HEADER
#    define EXT_META_IF_ALL_ANY_HEADER

#    include <type_traits>
namespace ext { namespace meta {

// REQUIRES GCC 8.0

// if_all  - logical and (fold expression)
template<bool... XS>
constexpr bool if_all_v = std::bool_constant<(XS && ...)>::value;

// if_any - logical or (fold expression)
template<bool... XS>
constexpr bool if_any_v = !if_all_v<!XS...>;

// enable if helper
template<typename T, bool... XS>
using enable_if_all_t = std::enable_if_t<(if_all_v<XS...>), T>;

template<typename T, bool... XS>
using enable_if_any_t = std::enable_if_t<(if_any_v<XS...>), T>;
}}     // namespace ext::meta
#endif // EXT_META_IF_ALL_ANY_HEADER
