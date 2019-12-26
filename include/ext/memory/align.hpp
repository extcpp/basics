// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_MEMORY_ALIGN_HEADER
#define EXT_MEMORY_ALIGN_HEADER

#include <ext/config.hpp>
#include <ext/util/bit_tricks.hpp>
#include <memory>
#include <new>

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
    return util::is_power_of_two(number);
}

}}     // namespace ext::memory
#endif // EXT_MEMORY_ALIGN_HEADER
