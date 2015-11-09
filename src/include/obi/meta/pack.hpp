// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <type_traits>

namespace obi { namespace meta {
    template <typename...>
    struct pack {};

    template <std::size_t N, typename T, typename Pack = pack<>>
    struct generate_pack;

    template <std::size_t N, typename T, typename... Args>
    struct generate_pack<N, T, pack<Args...>> {
        using type = typename generate_pack<N-1, T, pack<T, Args...>>::type;
    };

    template <typename T, typename... Args>
    struct generate_pack<0, T, pack<Args...>> {
        using type = pack<Args...>;
    };
}}
