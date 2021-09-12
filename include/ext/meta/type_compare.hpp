// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#ifndef EXT_META_TYPE_COMPARE_HEADER
#define EXT_META_TYPE_COMPARE_HEADER

#include <type_traits>

namespace ext { namespace meta {

// are same
template <class T, class... Ts>
struct are_same : std::conjunction<std::is_same<T, Ts>...> {};

template<class T, class... XXS>
inline constexpr bool are_same_v = are_same<T, XXS...>::value;

// is any
template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...> {};

template<class T, class... XXS>
inline constexpr bool is_any_v = is_any<T, XXS...>::value;

}}     // namespace ext::meta
#endif // EXT_META_TYPE_COMPARE_HEADER
