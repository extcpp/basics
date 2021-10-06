// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ENUM_HEADER
#define EXT_UTIL_ENUM_HEADER
#include <type_traits>

namespace ext::util {

namespace _detail_enum {
    // use underlying type if Type is a enum

    template<typename E1, typename E2>
    constexpr bool are_enums_v = std::is_enum_v<E1> && std::is_enum_v<E2>;

    template<typename E1, typename E2>
    constexpr bool is_same_underlying_v = std::is_same_v<std::underlying_type_t<E1>, std::underlying_type_t<E2>>;

    template <typename E, typename = void>
    inline constexpr bool is_fixed_enum = false;

    template <typename E>
    inline constexpr bool is_fixed_enum<E, std::void_t<decltype(E{0})>> = std::is_enum_v<E>;
} // namespace _detail_enum

/// check if enum is of fixed type
template <typename E>
inline constexpr bool is_fixed_enum_v = _detail_enum::is_fixed_enum<E>;

template <typename E>
struct is_fixed_enum : std::bool_constant<is_fixed_enum_v<E>> {};

/// to underlying
template<typename Enum>
inline constexpr std::underlying_type_t<Enum> enum_to_underlying(Enum const& e) noexcept {
    static_assert(std::is_enum_v<Enum>, "type is not an enum");
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template<typename T, typename Enum>
inline constexpr T enum_to_underlying_unsafe(Enum e) noexcept {
    static_assert(std::is_enum_v<Enum>, "type is not an enum");
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    return static_cast<T>(e);
}


/// to enum
template<typename Enum, typename T>
inline constexpr Enum underlying_to_enum(T value) noexcept {
    static_assert(std::is_enum_v<Enum>, "type is not an enum");
    static_assert(std::is_same_v<T, std::underlying_type_t<Enum>>, "type to convert from and underlying type are not the same");
    return static_cast<Enum>(value);
}

template<typename Enum, typename T>
inline constexpr Enum underlying_to_enum_unsafe(T value) noexcept {
    static_assert(std::is_enum_v<Enum>, "type is not an enum");
    static_assert(std::is_integral_v<T>, "type is not an integral type");
    return static_cast<Enum>(value);
}

/// enum to enum - one should be a subset of the other
template<typename EnumOut, typename EnumIn
        ,typename = std::enable_if_t<_detail_enum::are_enums_v<EnumIn, EnumOut> &&
                                     _detail_enum::is_same_underlying_v<EnumIn, EnumOut>>
        >
inline constexpr EnumOut enum_to_enum(EnumIn e) noexcept {
    return underlying_to_enum_unsafe<EnumOut>(enum_to_underlying(e));
}

template<typename EnumOut, typename EnumIn
        ,typename = std::enable_if_t<_detail_enum::are_enums_v<EnumIn, EnumOut>>
        >
inline constexpr EnumOut enum_to_enum_unsafe(EnumIn e) noexcept {
    return underlying_to_enum_unsafe<EnumOut>(enum_to_underlying(e));
}

}     // namespace ext::util
#endif // EXT_UTIL_ENUM_HEADER
