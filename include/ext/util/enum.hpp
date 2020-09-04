// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Copyright - 2019-2020 - Michael T. Becker
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ENUM_HEADER
#define EXT_UTIL_ENUM_HEADER
#include <type_traits>

namespace ext { namespace util {

namespace _detail {
// use underlying type if Type is a enum
template<typename E, typename U = std::underlying_type_t<E>>
using enable_if_enum_t = std::enable_if_t<std::is_enum_v<E>, U>;

template<typename E1, typename E2>
constexpr bool is_enums_v = std::is_enum_v<E1>&& std::is_enum_v<E2>;

template<typename E1, typename E2>
constexpr bool is_same_underlying_v = std::is_same_v<std::underlying_type_t<E1>, std::underlying_type_t<E1>>;

template <typename E, typename = E>
inline constexpr bool is_fixed_enum = false;

template <typename E>
inline constexpr bool is_fixed_enum<E, decltype(E{0})> = std::is_enum_v<E>;
} // namespace _detail

/// check if enum is of fixed type
template <typename E>
inline constexpr bool is_fixed_enum_v = _detail::is_fixed_enum<E>;

template <typename E>
struct is_fixed_enum : std::bool_constant<is_fixed_enum_v<E>> {};

/// to underlying
template<typename T, typename Enum, typename U = std::underlying_type_t<Enum>>
inline constexpr std::enable_if_t<std::is_enum_v<Enum> && std::is_same_v<T, U>, T> enum_to_underlying(Enum e) noexcept {
    return static_cast<U>(e);
}

template<typename Enum, typename U = std::underlying_type_t<Enum>>
inline constexpr _detail::enable_if_enum_t<Enum, U> enum_to_underlying_unsafe(Enum e) noexcept {
    return static_cast<U>(e);
}

/// to enum
template<typename Enum, typename T, typename U = std::underlying_type_t<Enum>>
inline constexpr std::enable_if_t<std::is_enum_v<Enum> && std::is_same_v<U, T>, Enum>
    underlying_to_enum(T value) noexcept {
    return static_cast<Enum>(value);
}

/// enum to enum
template<typename EnumOut, typename EnumIn>
inline constexpr std::enable_if_t<_detail::is_enums_v<EnumIn, EnumOut>, EnumOut>
    enum_to_enum_unsafe(EnumIn e) noexcept {
    return underlying_to_enum<EnumOut>(enum_to_underlying_unsafe(e));
}

template<typename EnumOut, typename EnumIn>
inline constexpr std::
    enable_if_t<_detail::is_enums_v<EnumIn, EnumOut> && _detail::is_same_underlying_v<EnumIn, EnumOut>, EnumOut>
    enum_to_enum(EnumIn e) noexcept {
    return underlying_to_enum<EnumOut>(enum_to_underlying_unsafe(e));
}


namespace operators::enumeration {

template<typename EnumType, typename IntegerType>
EnumType operator|(EnumType const& e, IntegerType const& i) {
    static_assert(std::is_enum_v<EnumType>);
    static_assert(std::is_same_v<std::underlying_type_t<EnumType>, IntegerType>);
    return static_cast<EnumType>(static_cast<IntegerType>(e) | i);
}

template<typename EnumType, typename IntegerType>
EnumType& operator|=(EnumType& e, IntegerType const& i) {
    static_assert(std::is_enum_v<EnumType>);
    e = e | i;
    return e;
}

template<typename EnumType>
EnumType operator|(EnumType const& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    using type = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>(static_cast<type>(e1) | static_cast<type>(e2));
}

template<typename EnumType>
EnumType& operator|=(EnumType& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    e1 = e1 | e2;
    return e1;
}

template<typename EnumType, typename IntegerType>
EnumType operator&(EnumType const& e, IntegerType const& i) {
    static_assert(std::is_enum_v<EnumType>);
    static_assert(std::is_same_v<std::underlying_type_t<EnumType>, IntegerType>);
    return static_cast<EnumType>(static_cast<IntegerType>(e) & i);
}

template<typename EnumType, typename IntegerType>
EnumType& operator&=(EnumType& e, IntegerType const& i) {
    static_assert(std::is_enum_v<EnumType>);
    e = e & i;
    return e;
}

template<typename EnumType>
EnumType operator&(EnumType const& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    using type = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>(static_cast<type>(e1) & static_cast<type>(e2));
}

template<typename EnumType>
EnumType& operator&=(EnumType& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    e1 = e1 & e2;
    return e1;
}

} // namespace operators::enumeration


// flag_set by Michael Becker
// motivated by http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
template<typename T>
struct flag_set {
    static_assert(std::is_enum_v<T>, "given type is not an enum");
    using underlying_type = std::underlying_type_t<T>;

    flag_set() : flags(0) {}

    flag_set(T f) : flags(static_cast<underlying_type>(f)) {}

    explicit flag_set(underlying_type flags_) : flags(flags_) {}

    bool operator()(T f) const {
        return (flags & static_cast<underlying_type>(f)) > 0;
    }

    flag_set& operator|=(flag_set other) {
        flags |= other.flags;
        return *this;
    }

    flag_set& operator&=(flag_set other) {
        flags &= other.flags;
        return *this;
    }

    flag_set& operator^=(flag_set other) {
        flags ^= other.flags;
        return *this;
    }

    flag_set& operator|=(T f) {
        flags |= static_cast<underlying_type>(f);
        return *this;
    }

    flag_set& operator&=(T f) {
        flags &= static_cast<underlying_type>(f);
        return *this;
    }

    flag_set& operator^=(T f) {
        flags ^= static_cast<underlying_type>(f);
        return *this;
    }

    underlying_type flags;
};

template<typename>
struct is_flags_enum : std::false_type {};

#define EXT_ENABLE_FLAG_SET_OPERATORS(x)         \
    namespace ext::util {                        \
    template<>                                   \
    struct is_flags_enum<x> : std::true_type {}; \
    }

template<typename T>
constexpr bool is_flags_enum_v = is_flags_enum<T>::value;

namespace operators::flag_set {
using ::ext::util::flag_set;

// --- flag_set operators ---
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator~(flag_set<T> fs) {
    return flag_set<T>(~fs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(flag_set<T> rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs.flags | lhs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(flag_set<T> rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs.flags & lhs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(flag_set<T> rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs.flags ^ lhs.flags);
}

// --- enum operators ---
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator~(T f) {
    return ~flag_set<T>(f);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(T rhs, T lhs) {
    return flag_set<T>(rhs) | flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(flag_set<T> rhs, T lhs) {
    return rhs | flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(T rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs) | lhs;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(T rhs, T lhs) {
    return flag_set<T>(rhs) & flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(flag_set<T> rhs, T lhs) {
    return rhs & flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(T rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs) & lhs;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(T rhs, T lhs) {
    return flag_set<T>(rhs) ^ flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(flag_set<T> rhs, T lhs) {
    return rhs ^ flag_set<T>(lhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(T rhs, flag_set<T> lhs) {
    return flag_set<T>(rhs) ^ lhs;
}
} // namespace operators::flag_set

}}     // namespace ext::util
#endif // EXT_UTIL_ENUM_HEADER
