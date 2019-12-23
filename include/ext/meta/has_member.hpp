// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#ifndef EXT_META_HAS_MEMBER_HEADER
#define EXT_META_HAS_MEMBER_HEADER

#include <type_traits>

namespace ext { namespace meta {

// has_value_var
template<typename T, typename = void>
struct has_var_value : std::false_type {};

template<typename T>
struct has_var_value<T, std::void_t<decltype(std::declval<T>().value)>> : std::true_type {};
// struct has_var_value<T, decltype(std::declval<T>().value, void())> :
// std::true_type {};  // warning unused var

template<typename T>
inline constexpr bool has_var_value_v = has_var_value<T>::value;

// has_type_member
template<class, class = void>
struct has_type_type : std::false_type {};

template<class T>
struct has_type_type<T, std::void_t<typename T::type>> : std::true_type {};
// struct has_type_type<T, decltype(typename T::type(),void())> : std::true_type
// {};

template<typename T>
inline constexpr bool has_type_type_v = has_type_type<T>::value;

// has_category_member
template<class, class = void>
struct has_type_category : std::false_type {};

template<class T>
struct has_type_category<T, std::void_t<typename T::category>> : std::true_type {};

template<typename T>
inline constexpr bool has_type_category_v = has_type_category<T>::value;

// is_input_iterator
template<class T, class = void>
struct is_input_iterator : std::false_type {};

template<class T>
struct is_input_iterator<T,
                         std::void_t<decltype(++std::declval<T&>()),                    // increment
                                     decltype(*std::declval<T&>()),                     // dereference
                                     decltype(std::declval<T&>() == std::declval<T&>()) // compare
                                     >> : std::true_type {};

template<typename T>
inline constexpr bool is_input_iterator_v = is_input_iterator<T>::value;
}}     // namespace ext::meta
#endif // EXT_META_HAS_MEMBER_HEADER
