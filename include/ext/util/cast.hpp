// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_CAST_HEADER
#define EXT_UTIL_CAST_HEADER
#include <array>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace ext { namespace util {

//// size calcuation for important types
template<typename T, std::size_t N>
inline constexpr std::size_t size(T (&)[N]) {
    return N;
}

namespace _detail {
template<typename T>
inline constexpr std::size_t size_of() {
    return (sizeof(T));
}

template<typename T>
inline constexpr std::size_t size_of(T const& arg) {
    return (sizeof(decltype(arg)));
}

template<typename T, std::size_t N>
inline constexpr std::size_t size_of() {
    return (sizeof(T) * N);
}

template<typename T, std::size_t N>
inline constexpr std::size_t size_of(T (&)[N]) {
    return N * size_of<T>();
}

template<typename T, std::size_t N>
inline constexpr std::size_t size_of(std::array<T, N> const&) {
    return (sizeof(T) * N);
}

template<typename C, typename T, typename A>
inline constexpr std::size_t size_of(std::basic_string<C, T, A> const& arg) {
    return (sizeof(T) * arg.size());
}

} // namespace _detail

template<typename... T>
inline constexpr std::size_t size_of() {
    return (_detail::size_of<T>() + ...);
}

template<typename... T>
inline constexpr std::size_t size_of(T const&... args) {
    return (_detail::size_of(args) + ...);
}

// use this if not all args size_of functions
// are constexpr
// template <typename ...T> inline
// std::size_t size_of_all(T const& ...args){
//    return (_detail::size_of(args) + ...);
//}

//// singed <-> unsingend - conversion

template<typename T, typename S = std::make_unsigned_t<T>>
constexpr S to_unsigned(T in) {
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    static_assert(std::is_signed_v<T>, "type is not singed");

    return S(in);
}

template<typename T, typename S = std::make_signed_t<T>>
constexpr S to_signed(T in) {
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    static_assert(std::is_unsigned_v<T>, "type is not unsinged");

    return S(in);
}

template<typename T, typename S = std::make_unsigned_t<T>>
constexpr S to_unsigned_checked(T in) {
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    static_assert(std::is_signed_v<T>, "type is not singed");

    if (in < T(std::numeric_limits<S>::min())) {
        throw std::logic_error("conversion to_unsigend not possible");
    }
    return S(in);
}

template<typename T, typename S = std::make_signed_t<T>>
constexpr S to_signed_checked(T in) {
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    static_assert(std::is_unsigned_v<T>, "type is not unsinged");

    if (in > T(std::numeric_limits<S>::max())) {
        throw std::logic_error("conversion to_sigend not possible");
    }
    return S(in);
}

//// standard conversions - potentially different sizes

template<typename To, typename From>
std::enable_if_t<!std::is_array<To>::value, void> convert(const From& from, To& to) {
    std::memcpy(&to, &from, sizeof(To));
}

template<typename To, typename From, std::size_t length>
std::enable_if_t<std::is_array<To>::value, void> convert(const From& from, To (&to)[length]) {
    std::memcpy(&to, &from, sizeof(To) * length);
}

template<typename To, typename From>
constexpr inline To convert_checked(const From& from) {
    static_assert(sizeof(To) == sizeof(From), "type_cast requires types with equal size");
    To to;
    std::memcpy(&to, &from, sizeof(To));
    return to;
}

template<typename To, typename From>
constexpr inline To convert_to_bigger(const From& from) {
    // copy `from` size form `from` to `to` and fill rest with 0
    // FFFF      -> 0000 0000 => 0000 FFFF

    static_assert(sizeof(To) >= sizeof(From), "type_cast requires target with greater or equal size");

    To to = To(0);
    std::memcpy(&to, &from, sizeof(From));
    return to;
}

template<typename To, typename From>
constexpr inline To convert_to_smaller(const From& from) {
    // copy `from` size form `from` to `to` and fill rest with 0
    // 0000 FFFF -> 0000 => FFFF

    static_assert(sizeof(To) <= sizeof(From), "type_cast requires target with less or equal size");

    To to;
    std::memcpy(&to, &from, sizeof(To));
    return to;
}

template<typename To, typename From>
constexpr inline To convert_different(const From& from) {
    // if `to` is less equal only copy `to` size form `from` to `to`
    // otherwise copy `from` size form `from` to `to` and fill rest with 0
    // 0000 FFFF ->      0000 =>      FFFF
    // FFFF      -> 0000 0000 => 0000 FFFF

    To to = To(0);
    std::memcpy(&to, &from, (sizeof(To) <= sizeof(From)) ? sizeof(To) : sizeof(From));
    return to;
}

//// end - conversion

}}     // namespace ext::util
#endif // EXT_UTIL_CAST_HEADER
