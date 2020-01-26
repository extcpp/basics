// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <type_traits>
#ifndef EXT_UTIL_BIT_TRICKS_HEADER
    #define EXT_UTIL_BIT_TRICKS_HEADER

namespace ext { namespace util {

template<typename T>
constexpr inline T lsb(T number) {
    static_assert(-1 == ~0, "not 2's complement");
    return (number & -number);
}

template<typename T>
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> inline constexpr is_power_of_two(T number) {
    // alignment is expected to be a power of 2
    return (number != 0) && ((number & (number - 1)) == 0);
}

}}     // namespace ext::util
#endif // EXT_UTIL_BIT_TRICKS_HEADER
