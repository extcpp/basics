// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_UTIL_SHOW_HEADER
#    define EXT_UTIL_SHOW_HEADER
#    include "../meta/has_member.hpp"
#    include "container_helper.hpp"
#    include <iostream>
#    include <sstream>
#    include <string>
#    include <type_traits>

namespace ext { namespace util {

template<typename Container>
inline std::enable_if_t<
    ext::meta::has_type_category_v<_detail::container_traits<Container>> &&
        std::is_same_v<typename _detail::container_traits<Container>::category, _detail::associative_tag>,
    std::ostream&>
    operator<<(std::ostream& out, const Container& container);

template<typename Container>
inline
    // if it is a container, that is mentioned in my container traits
    std::enable_if_t<
        ext::meta::has_type_category_v<_detail::container_traits<Container>> &&
            !std::is_same_v<typename _detail::container_traits<Container>::category, _detail::associative_tag>,
        std::ostream&>
    operator<<(std::ostream& out, const Container& container) {
    using namespace std;
    out << "[";
    if (container.size() == 1) {
        out << *container.begin();
    }
    if (container.size() > 1) {
        auto current = container.begin();
        while (next(current) != container.end()) {
            out << *current << ",";
            current++;
        }
        out << *current;
    }
    out << "]";
    return out;
}

template<typename Key, typename Value>
inline std::ostream& out_pair_in_map(std::ostream& out, const std::pair<Key, Value>& pair) {
    using namespace std;
    out << pair.first << "=" << pair.second;
    return out;
}

template<typename Container>
inline std::enable_if_t<
    ext::meta::has_type_category_v<_detail::container_traits<Container>> &&
        std::is_same_v<typename _detail::container_traits<Container>::category, _detail::associative_tag>,
    std::ostream&>
    operator<<(std::ostream& out, const Container& container) {
    using namespace std;
    out << "{";
    if (container.size() == 1) {
        out_pair_in_map(out, *container.begin());
    }
    if (container.size() > 1) {
        auto current = container.begin();
        while (next(current) != container.end()) {
            out_pair_in_map(out, *current) << ",";
            current++;
        }
        out_pair_in_map(out, *current);
    }
    out << "}";
    return out;
}

template<typename Key, typename Value>
inline std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value>& pair) {
    using namespace std;
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
