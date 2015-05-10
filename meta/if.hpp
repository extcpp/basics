// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <type_traits>
namespace obi { namespace meta {

// meta_if
    template<typename, typename first, typename second>
    struct meta_if {
        using type = first;
    };

    template<typename first, typename second>
    struct meta_if<std::false_type, first, second> {
        using type = second;
    };

    template<typename cond, typename first, typename second>
    using meta_if_t = typename meta_if<cond, first, second>::type;


// meta_ifc
    template<bool, typename first, typename second>
    struct meta_ifc {
        using type = first;
    };

    template<typename first, typename second>
    struct meta_ifc<false, first, second> {
        using type = second;
    };

    template<bool cond, typename first, typename second>
    using meta_ifc_t = typename meta_ifc<cond, first, second>::type;

}}  // namespace obi::meta
