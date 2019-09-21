// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_MEMORY_ALIGN_HEADER
#define EXT_MEMORY_ALIGN_HEADER

#include <ext/config.hpp>
#include <memory>

namespace ext { namespace memory {

inline constexpr std::size_t cache_line_size() {
#ifdef EXT_KNOWN_L1_CACHE_LINE_SIZE
    return EXT_KNOWN_L1_CACHE_LINE_SIZE;
#else
    return ::std::hardware_destructive_interference_size;
#endif // EXT_KNOWN_L1_CACHE_LINE_SIZE
}

template<typename T>
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> inline constexpr is_alignment(T number) {
    // alignment is expected to be a power of 2
    return (number != 0) && ((number & (number - 1)) == 0);
}
}}     // namespace ext::memory
#endif // EXT_MEMORY_ALIGN_HEADER
