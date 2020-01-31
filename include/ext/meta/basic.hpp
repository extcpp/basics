// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_META_BASIC_HEADER
#define EXT_META_BASIC_HEADER
#include <type_traits>

namespace ext::meta {

template<typename T>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;


// if_bool/integer_constant
template<typename, typename first, typename second>
struct if_constant {
    using type = first;
};

template<typename first, typename second>
struct if_constant<std::false_type, first, second> {
    using type = second;
};

template<typename cond, typename first, typename second>
using if_constant_t = typename if_constant<cond, first, second>::type;

// if_bool
template<bool cond, typename first, typename second>
using if_bool_t = std::conditional_t<cond, first, second>;


// if_all  - logical and (fold expression)
template<bool... XS>
constexpr bool if_all_v = std::bool_constant<(XS && ...)>::value;

// enable_if_all
template<typename T, bool... XS>
using enable_if_all_t = std::enable_if_t<(if_all_v<XS...>), T>;

// if_any - logical or (fold expression)
template<bool... XS>
constexpr bool if_any_v = !if_all_v<!XS...>;

// enable_if_any
template<typename T, bool... XS>
using enable_if_any_t = std::enable_if_t<(if_any_v<XS...>), T>;

} // namespace ext::meta
#endif // EXT_META_BASIC_HEADER
