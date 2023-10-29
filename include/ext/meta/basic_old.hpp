// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_META_BASIC_OLD_HEADER
#define EXT_META_BASIC_OLD_HEADER
#include <type_traits>

namespace ext::meta {

// std::remove_cv_ref_t
template <typename T>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;


// std::conditional
template <typename, typename first, typename second>
struct if_constant {
    using type = first;
};
template <typename first, typename second>
struct if_constant<std::false_type, first, second> {
    using type = second;
};
template <typename cond, typename first, typename second>
using if_constant_t = typename if_constant<cond, first, second>::type;

} // namespace ext::meta
#endif // EXT_META_BASIC_OLD_HEADER
