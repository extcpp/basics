// Copyright - 2016-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_PRETTY_HEADER
#define EXT_UTIL_PRETTY_HEADER
#include <iostream>
#include <sstream>
#include <tuple>
#include <type_traits>

#include <ext/util/container_traits.hpp>

namespace ext::util {

namespace pretty {
// function to print containers
template<typename T>
inline std::enable_if_t<_detail::is_container<T>::value, std::ostream&> operator<<(std::ostream& out,
                                                                                   const T& container);
}

// functions that add quoting in containers
namespace _detail::pretty {
//string_view
inline std::ostream& operator<<(std::ostream& out, std::string_view const& str) {
    using namespace std::literals::string_literals;
    std::operator<<(out, "\""s);
    std::operator<<(out, str);
    std::operator<<(out, "\""s);
    return out;
}

//string
inline std::ostream& operator<<(std::ostream& out, std::string const& str) {
    using namespace std::literals::string_literals;
    std::operator<<(out, "\""s);
    std::operator<<(out, str);
    std::operator<<(out, "\""s);
    return out;
}

//c-string
inline std::ostream& operator<<(std::ostream& out, char const* str) {
    using namespace std::literals::string_literals;
    std::operator<<(out, "\""s);
    std::operator<<(out, str);
    std::operator<<(out, "\""s);
    return out;
}

//pair
template<typename Key, typename Value>
inline std::ostream& out_pair_in_map(std::ostream& out, const std::pair<Key, Value>& pair) {
    using namespace std::literals::string_literals;
    using ext::util::_detail::pretty::operator<<;
    using ext::util::pretty::operator<<; // it could be anohter container
    out << pair.first;
    std::operator<<(out, ":"s) << pair.second;
    return out;
}

template<typename Container, typename T>
inline std::ostream& fmt_internal(std::ostream& out, T&& value) {
    using ext::util::_detail::pretty::operator<<;
    if constexpr (_detail::is_associative<Container>::value) {
        out_pair_in_map(out, std::forward<T>(value));
    } else {
        using ext::util::_detail::pretty::operator<<;
        using ext::util::pretty::operator<<;
        out << std::forward<T>(value);
    }
    return out;
}

} // namespace _detail


namespace pretty {
// diverse container
template<typename T>
inline std::enable_if_t<_detail::is_container<T>::value, std::ostream&> operator<<(std::ostream& out,
                                                                                   const T& container) {
    using namespace std::literals::string_literals;

    if constexpr (_detail::is_associative<T>::value || _detail::is_set<T>::value) {
        std::operator<<(out, "{"s);
    } else {
        std::operator<<(out, "["s);
    }
    auto size = container.size();
    if (size > 0) {
        auto current = container.begin();
        if (container.size() > 1) {
            while (next(current) != container.end()) {
                using ext::util::_detail::pretty::operator<<;
                _detail::pretty::fmt_internal<T>(out, *current);
                std::operator<<(out, ", "s);
                current++;
            }
        }
        _detail::pretty::fmt_internal<T>(out, *current);
    } else {
        std::operator<<(out, " "s);
    }

    if constexpr (_detail::is_associative<T>::value || _detail::is_set<T>::value) {
        std::operator<<(out, "}"s);
    } else {
        std::operator<<(out, "]"s);
    }

    return out;
}
// diverse container - end


// pair outside of container
template<typename Key, typename Value>
inline std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value>& pair) {
    using namespace std::literals::string_literals;
    using ext::util::_detail::pretty::operator<<;
    std::operator<<(out, "("s) << pair.first;
    std::operator<<(out, ", "s) << pair.second;
    std::operator<<(out, ")"s);
    return out;
}
// pair outside of container - end

// tuple
template<typename Tuple, std::size_t... I>
inline std::ostream& fmt(std::ostream& out, const Tuple& tuple, std::index_sequence<I...>) {
    using namespace std::literals::string_literals;

    std::operator<<(out, "("s);
    using ext::util::_detail::pretty::operator<<;
    ((std::operator<<(out, (I == 0 ? ""s : ", "s)) << std::get<I>(tuple)), ...);
    std::operator<<(out, ")"s);
    return out;
}

inline std::ostream& fmt(std::ostream& out, const std::tuple<>&, std::index_sequence<>) {
    using namespace std::literals::string_literals;
    std::operator<<(out, "( )"s);
    return out;
}

template<typename... T>
inline constexpr std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tuple) {
    return fmt(out, tuple, std::make_index_sequence<sizeof...(T)>());
}
// tuple - end

template<typename T>
inline std::string fmt(T&& item) {
    std::stringstream ss;
    ss << std::boolalpha << std::forward<T>(item);
    return ss.str();
}

} // namespace pretty - end

}     // namespace ext::util
#endif // EXT_UTIL_PRETTY_HEADER
