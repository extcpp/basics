// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_META_PACK_HEADER
#define EXT_META_PACK_HEADER

#include <type_traits>

namespace ext { namespace meta {
    //generate pack of length N and type T
    template <typename...>
    struct pack {};

    //create first element - this is used by user
    template <std::size_t N, typename T, typename Pack = pack<>>
    struct generate_pack;

    //recursive pack creation
    template <std::size_t N, typename T, typename... Args>
    struct generate_pack<N, T, pack<Args...>> {
        using type = typename generate_pack<N-1, T, pack<T, Args...>>::type;
    };
    //recursion end
    template <typename T, typename... Args>
    struct generate_pack<0, T, pack<Args...>> {
        using type = pack<Args...>;
    };

    //14 style access
    template <std::size_t N, typename T>
    using generate_pack_t = typename generate_pack<N, T>::type;
}}
#endif // EXT_META_PACK_HEADER
