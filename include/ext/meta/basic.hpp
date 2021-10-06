// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_META_BASIC_HEADER
#define EXT_META_BASIC_HEADER
#include <type_traits>

namespace ext::meta {

// create conjunction or disjunction of multiple types
// if_all  - logical and (fold expression)
template<bool... XS>
constexpr bool if_all_v = std::bool_constant<(XS && ...)>::value;

// if_any - logical or (fold expression)
template<bool... XS>
constexpr bool if_any_v = !if_all_v<!XS...>;


//// enable_if for more than one condition
// enable_if_all
template<typename T, bool... XS>
using enable_if_all_t = std::enable_if_t<(if_all_v<XS...>), T>;

// enable_if_any
template<typename T, bool... XS>
using enable_if_any_t = std::enable_if_t<(if_any_v<XS...>), T>;


//// check if all or any type matches the first type
// are same
template <class T, class... Ts>
inline constexpr bool are_same_v = if_all_v<std::is_same_v<T, Ts> ...>;

// is any
template<class T, class... Ts>
inline constexpr bool is_any_v = if_any_v<std::is_same_v<T, Ts> ...>;


} // namespace ext::meta
#endif // EXT_META_BASIC_HEADER
