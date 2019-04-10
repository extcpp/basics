// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MEMORY_TAGGED_ALIGN_HEADER
#define OBI_MEMORY_TAGGED_ALIGN_HEADER

#include <obi/config.hpp>
#include <memory>

namespace obi { namespace memory {

inline constexpr std::size_t cache_line_size() {
#ifdef OBI_KNOWN_L1_CACHE_LINE_SIZE
    return OBI_KNOWN_L1_CACHE_LINE_SIZE;
#else
    return ::std::hardware_destructive_interference_size;
#endif
}

template <typename T>
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool>
inline constexpr
is_alignment(T number) {
    // alignment is expected to be a power of 2
    return (number != 0) && ( (number & (number - 1)) == 0 );
}

}}
#endif
