// Copyright - 2016-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_SHOW_HEADER
#define EXT_UTIL_SHOW_HEADER
#include <type_traits> // forward declares std::tuple
#include <iostream>
#include <sstream>

// try to forarad declare std::string
#if __has_include(<bits/stringfwd.h>)
#include <bits/stringfwd.h>
#elif __has_include(<iosfwd>)
#include <iosfwd>
#else
#include <string>
#endif

#include "container_traits.hpp"

namespace ext { namespace util {

// function to print containers
template<typename T> inline
std::enable_if_t<_detail::is_container<T>::value, std::ostream&>
    operator<<(std::ostream& out, const T& container);

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
