// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Copyright - 2019-2020 - Michael T. Becker
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_FLAG_SET_HEADER
#define EXT_UTIL_FLAG_SET_HEADER
#include <type_traits>

namespace ext { namespace util {

namespace _detail_flag_set {
template<typename, typename = void>
struct is_flags_enum : std::false_type { };

template<typename T>
struct is_flags_enum<T, std::void_t<decltype(is_flags_enum(std::declval<T>()))>> : std::true_type {};

} // namespace _detail_flag_set

#define EXT_ENABLE_FLAG_SET_OPERATORS(x)         \
    namespace ext::util::_detail_flag_set {      \
    template<>                                   \
    struct is_flags_enum<x> : std::true_type {}; \
    }

template<typename T>
constexpr bool is_flags_enum_v = _detail_flag_set::is_flags_enum<T>::value;

template<typename T>
struct flag_set;

// flag_set by Michael Becker
// motivated by http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
template<typename T>
struct flag_set {
    static_assert(std::is_enum_v<T>, "given type is not an enum");
    using underlying_type = std::underlying_type_t<T>;

    flag_set() : flags(0) {}

    flag_set(T f) : flags(static_cast<underlying_type>(f)) {}

    explicit flag_set(underlying_type flags_) : flags(flags_) {}

    // add
    flag_set& add(flag_set const& f) {
        this->flags = static_cast<underlying_type>(this->flags | f.flags);
        return *this;
    }

    flag_set& add(T const& f) {
        this->flags = static_cast<underlying_type>(this->flags | static_cast<underlying_type>(f));
        return *this;
    }

    // remove
    flag_set& remove(flag_set const& f) {
        this->flags = static_cast<underlying_type>(this->flags & static_cast<underlying_type>(~f.flags));
        return *this;
    }

    flag_set& remove(T const& f) {
        this->flags = static_cast<underlying_type>(this->flags & static_cast<underlying_type>(~ static_cast<underlying_type>(f)));
        return *this;
    }


    // contains
    bool contains(flag_set const& f) {
        return static_cast<underlying_type>(this->flags & f.flags) == f.flags;
    }

    bool contains(T const& f) {
        return (this->flags & static_cast<underlying_type>(f)) == static_cast<underlying_type>(f);
    }

    // equal
    bool equal(flag_set const& f) {
        return this->flags == f.flags;
    }

    bool equal(T const& f) {
        return this->flags == static_cast<underlying_type>(f);
    }

    // REVIEW - Mic why did you create this one?
    //bool operator()(T f) const {
    //    return (flags & static_cast<underlying_type>(f)) > 0;
    //}

    underlying_type flags;
};


// --- flag_set operators ---
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
bool operator==(flag_set<T> const& rhs, flag_set<T> const& lhs) {
    return rhs.flags == lhs.flags;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
bool operator!=(flag_set<T> const& rhs, flag_set<T> const& lhs) {
    return rhs.flags != lhs.flags;
}



// op |
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(flag_set<T> lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs.flags | rhs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|=(flag_set<T>& lhs, flag_set<T> rhs) {
    lhs.flags = (lhs.flags | rhs.flags);
    return lhs;
}



// op &
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(flag_set<T> lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs.flags & rhs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&=(flag_set<T>& lhs, flag_set<T> rhs) {
    lhs.flags = (lhs.flags & rhs.flags);
    return lhs;
}



// op ^
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(flag_set<T> lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs.flags ^ rhs.flags);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^=(flag_set<T>& lhs, flag_set<T> rhs) {
    lhs.flags = (lhs.flags ^ rhs.flags);
    return lhs;
}



// op ~
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator~(flag_set<T> fs) {
    flag_set<T> rv;
    rv.flags = static_cast<std::underlying_type_t<T>>(~fs.flags);
    return rv;
}

// ----------------------------------------------------------------------------
// --- enum operators ---
// ----------------------------------------------------------------------------

// op |
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(T lhs, T rhs) {
    return flag_set<T>(lhs) | flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(flag_set<T> lhs, T rhs) {
    return lhs | flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|(T lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs) | rhs;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator|=(flag_set<T>& lhs, T rhs) {
    lhs = lhs | flag_set<T>(rhs);
    return lhs;
}



// op &
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(T lhs, T rhs) {
    return flag_set<T>(lhs) & flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(flag_set<T> lhs, T rhs) {
    return lhs & flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&(T lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs) & rhs;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator&=(flag_set<T>& lhs, T rhs) {
    lhs = lhs & flag_set<T>(rhs);
    return lhs;
}



// op ^
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(T lhs, T rhs) {
    return flag_set<T>(lhs) ^ flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(flag_set<T> lhs, T rhs) {
    return lhs ^ flag_set<T>(rhs);
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^(T lhs, flag_set<T> rhs) {
    return flag_set<T>(lhs) ^ rhs;
}

template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator^=(flag_set<T>& lhs, T rhs) {
    lhs = lhs ^ flag_set<T>(rhs);
    return lhs;
}



// op ~
template<typename T, typename = std::enable_if_t<is_flags_enum_v<T>>>
flag_set<T> operator~(T f) {
    return ~flag_set<T>(f);
}


}}     // namespace ext::util
#endif // EXT_UTIL_FLAG_SET_HEADER
