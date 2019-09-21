// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_UTIL_ENUM_HEADER
#    define EXT_UTIL_ENUM_HEADER

#    include <type_traits>

namespace ext { namespace util {

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
}}     // namespace ext::util
#endif // EXT_UTIL_ENUM_HEADER
