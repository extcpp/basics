// Copyright - 2016-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_SHOW_HEADER
#define EXT_UTIL_SHOW_HEADER
#include <type_traits>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace ext { namespace util {

inline std::ostream& operator<<(std::ostream& out, std::string& str) {
    using namespace std::literals::string_literals;
    std::operator<<(out,"\""s);
    std::operator<<(out,str);
    std::operator<<(out,"\""s);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, std::string const& str) {
    using namespace std::literals::string_literals;
    std::operator<<(out,"\""s);
    std::operator<<(out,str);
    std::operator<<(out,"\""s);
    return out;
}

namespace _detail {

template<typename T, typename = void>
struct has_iterator : std::false_type {};

template<typename T>
struct has_iterator<T, std::void_t< typename T::iterator
                                  , typename T::const_iterator
                                  , decltype(std::declval<T&&>().begin())
                                  , decltype(std::declval<T&&>().end())
                                  >> : std::true_type {};

template<typename T, class ...TT >
struct is_excluded  : std::bool_constant< (std::is_same<std::decay_t<T> ,std::decay_t<TT> >::value || ... ) > {};

template<typename T>
struct is_container : std::bool_constant<has_iterator<std::decay_t<T>>::value && !is_excluded<T, std::string, std::string_view>::value > {};

template<typename T, typename = void>
struct is_associative : std::false_type {};

template<typename T>
struct is_associative<T, std::void_t< typename std::decay_t<T>::key_type, typename std::decay_t<T>::mapped_type >> : std::true_type {};

template<typename T> inline
std::enable_if_t<_detail::is_container<T>::value, std::ostream&>
    operator<<(std::ostream& out, const T& container);

template<typename Key, typename Value>
inline std::ostream& out_pair_in_map(std::ostream& out, const std::pair<Key, Value>& pair) {
    using ext::util::operator<<;
    out << pair.first << ":" << pair.second;
    return out;
}

template<typename Container, typename T>
inline std::ostream& show(std::ostream& out, T&& value) {
    using ext::util::operator<<;
	if constexpr(_detail::is_associative<Container>::value) {
		out_pair_in_map(out, std::forward<T>(value));
	} else {
		out << std::forward<T>(value);
	}
    return out;
}
} // namespace _detail

template<typename T> inline
std::enable_if_t<_detail::is_container<T>::value, std::ostream&>
    operator<<(std::ostream& out, const T& container) {
	using namespace std::literals::string_literals;

	if constexpr (_detail::is_associative<T>::value) {
        std::operator<<(out,"{"s);
	} else {
        std::operator<<(out,"["s);
	}

    auto current = container.begin();
    if (container.size() > 1) {
        while (next(current) != container.end()) {
			_detail::show<T>(out, *current);
            std::operator<<(out,","s);
            current++;
        }
    }
	_detail::show<T>(out, *current);

	if constexpr (_detail::is_associative<T>::value) {
        std::operator<<(out,"}"s);
	} else {
        std::operator<<(out,"]"s);
	}

    return out;
}

template<typename Key, typename Value>
inline std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value>& pair) {
    using namespace std::literals::string_literals;
    out << "(" << pair.first << "," << pair.second << ")";
    return out;
}

template<typename T>
inline std::string show(T item) {
    std::stringstream ss;
    ss << item;
    return ss.str();
}
}}     // namespace ext::util
#endif // EXT_UTIL_SHOW_HEADER
