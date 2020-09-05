// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ENUM_OPERATIONS_HEADER
#define EXT_UTIL_ENUM_OPERATIONS_HEADER
#include <ext/util/enum.hpp>

namespace ext { namespace util {

// This code is incomplete and its application is questionable.
namespace operators::enumeration {

/// operator | ////////////////////////////////////////////////////////////////
// e|i -- i|e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator|(EnumType const& e, IntegerType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) | i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator|(IntegerType const& e, EnumType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) | i);
}

// e|e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator|(EnumType const& e1, EnumType const& e2) {
    using type = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>(static_cast<type>(e1) | static_cast<type>(e2));
}

// e|=i -- i|=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator|=(EnumType& e, IntegerType const& i) {
    e = e | i;
    return e;
}
// REVIEW -- Do we want this operation?
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator|=(IntegerType& i, EnumType const& e) {
    i = i | enum_to_underlying(e);
    return i;
}

// e|=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator|=(EnumType& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    e1 = e1 | e2;
    return e1;
}

/// operator & ////////////////////////////////////////////////////////////////
// e&i -- i&e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator&(EnumType const& e, IntegerType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) & i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator&(IntegerType const& e, EnumType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) & i);
}

// e&e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator&(EnumType const& e1, EnumType const& e2) {
    using type = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>(static_cast<type>(e1) & static_cast<type>(e2));
}

// e&=i -- i&=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator&=(EnumType& e, IntegerType const& i) {
    e = e & i;
    return e;
}
// REVIEW -- Do we want this operation?
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator&=(IntegerType& i, EnumType const& e) {
    i = i & enum_to_underlying(e);
    return i;
}

// e&=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator&=(EnumType& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    e1 = e1 & e2;
    return e1;
}

/// operator ^ ////////////////////////////////////////////////////////////////
// e^i -- i^e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator^(EnumType const& e, IntegerType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) ^ i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator^(IntegerType const& e, EnumType const& i) {
    return static_cast<EnumType>(static_cast<IntegerType>(e) ^ i);
}

// e^e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator^(EnumType const& e1, EnumType const& e2) {
    using type = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>(static_cast<type>(e1) ^ static_cast<type>(e2));
}

// e^=i -- i^=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator^=(EnumType& e, IntegerType const& i) {
    e = e ^ i;
    return e;
}
// REVIEW -- Do we want this operation?
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType> &&
                    std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator^=(IntegerType& i, EnumType const& e) {
    i = i ^ enum_to_underlying(e);
    return i;
}

// e^=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator^=(EnumType& e1, EnumType const& e2) {
    static_assert(std::is_enum_v<EnumType>);
    e1 = e1 ^ e2;
    return e1;
}

/// operator ~ ////////////////////////////////////////////////////////////////
// ~e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator~(EnumType const& e) {
    return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(e));
}

} // namespace operators::enumeration

}}     // namespace ext::util
#endif // EXT_UTIL_ENUM_OPERATIONS_HEADER
