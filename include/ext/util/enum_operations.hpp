// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ENUM_OPERATIONS_HEADER
#define EXT_UTIL_ENUM_OPERATIONS_HEADER
#include <ext/util/enum.hpp>

namespace ext { namespace util {

// This code is incomplete and its application is questionable.
namespace operators::enumeration {

/// operator & ////////////////////////////////////////////////////////////////
// e&e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator&(EnumType const& e1, EnumType const& e2) {
    return underlying_to_enum_unsafe<EnumType>(enum_to_underlying(e1) & enum_to_underlying(e2));
}

// e&i -- i&e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator&(EnumType const& e, IntegerType const& i) {
    return e & underlying_to_enum<EnumType>(i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator&(IntegerType const& i, EnumType const& e) {
    return underlying_to_enum<EnumType>(i) & e;
}

// e&=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator&=(EnumType& e1, EnumType const& e2) {
    e1 = e1 & e2;
    return e1;
}

// e&=i -- i&=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator&=(EnumType& e, IntegerType const& i) {
    e = e & underlying_to_enum<EnumType>(i);
    return e;
}
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator&=(IntegerType& i, EnumType const& e) {
    i = i & enum_to_underlying<EnumType>(e);
    return i;
}

/// operator | ////////////////////////////////////////////////////////////////
// e|e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator|(EnumType const& e1, EnumType const& e2) {
    return underlying_to_enum_unsafe<EnumType>(enum_to_underlying(e1) | enum_to_underlying(e2));
}

// e|i -- i|e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator|(EnumType const& e, IntegerType const& i) {
    return e | underlying_to_enum<EnumType>(i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator|(IntegerType const& i, EnumType const& e) {
    return underlying_to_enum<EnumType>(i) | e;
}

// e|=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator|=(EnumType& e1, EnumType const& e2) {
    e1 = e1 | e2;
    return e1;
}

// e|=i -- i|=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator|=(EnumType& e, IntegerType const& i) {
    e = e | underlying_to_enum<EnumType>(i);
    return e;
}
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator|=(IntegerType& i, EnumType const& e) {
    i = i | enum_to_underlying<EnumType>(e);
    return i;
}

/// operator ^ ////////////////////////////////////////////////////////////////
// e^e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType operator^(EnumType const& e1, EnumType const& e2) {
    return underlying_to_enum_unsafe<EnumType>(enum_to_underlying(e1) ^ enum_to_underlying(e2));
}

// e^i -- i^e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator^(EnumType const& e, IntegerType const& i) {
    return e ^ underlying_to_enum<EnumType>(i);
}

template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType operator^(IntegerType const& i, EnumType const& e) {
    return underlying_to_enum<EnumType>(i) ^ e;
}

// e^=e
template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
EnumType& operator^=(EnumType& e1, EnumType const& e2) {
    e1 = e1 ^ e2;
    return e1;
}

// e^=i -- i^=e
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
EnumType& operator^=(EnumType& e, IntegerType const& i) {
    e = e ^ underlying_to_enum<EnumType>(i);
    return e;
}
template<typename EnumType, typename IntegerType
        ,typename = std::enable_if_t<std::is_enum_v<EnumType>>
        ,typename = std::enable_if_t<std::is_same_v<std::underlying_type_t<EnumType>,IntegerType>>
        >
IntegerType& operator^=(IntegerType& i, EnumType const& e) {
    i = i ^ enum_to_underlying<EnumType>(e);
    return i;
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
