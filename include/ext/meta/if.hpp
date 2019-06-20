// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_META_IF_HEADER
#define EXT_META_IF_HEADER

#include <type_traits>
namespace ext { namespace meta {

// if first value / expession is true the `first` type is returned otherwise
// the `second`

// if_
    template<typename, typename first, typename second>
    struct if_ {
        using type = first;
    };

    template<typename first, typename second>
    struct if_<std::false_type, first, second> {
        using type = second;
    };

    template<typename cond, typename first, typename second>
    using if_t = typename if_<cond, first, second>::type;


// if_c
    template<bool, typename first, typename second>
    struct if_c {
        using type = first;
    };

    template<typename first, typename second>
    struct if_c<false, first, second> {
        using type = second;
    };

    template<bool cond, typename first, typename second>
    using if_c_t = typename if_c<cond, first, second>::type;

}}  // namespace ext::meta
#endif // EXT_META_IF_HEADER
