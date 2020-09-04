// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Copyright - 2019-2020 - Michael T. Becker
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ENUM_HEADER
#define EXT_UTIL_ENUM_HEADER
#include <type_traits>

namespace ext { namespace util {

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
