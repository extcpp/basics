// Copyright - 2016-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
// Idea and code from STL
// http://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-3-of-n

// TODO: Is this useful for people without brain damage? Discuss with Mic.
#ifndef EXT_UTIL_CONTAINER_HELPER_HEADER
#define EXT_UTIL_CONTAINER_HELPER_HEADER
#include <ext/util/container_tags.hpp>

#include <algorithm>

namespace ext { namespace util {


// vector like
template<typename Container, typename X>
enable_if_container_has_tag_t<Container, _detail::vectorlike_tag> erase(Container& c, const X& x) {
    c.erase(std::remove(c.begin(), c.end(), x), c.end());
}

template<typename Container, typename Predicate>
enable_if_container_has_tag_t<Container, _detail::vectorlike_tag> erase_if(Container& c, Predicate p) {
    c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
}

template<typename Container, class Compare>
enable_if_container_has_tag_t<Container, _detail::vectorlike_tag> sort_comp(Container& c, Compare comp) {
    std::sort(c.begin(), c.end(), comp);
}

template<typename Container>
enable_if_container_has_tag_t<Container, _detail::vectorlike_tag> sort_helper_comp(Container& c) {
    std::sort(c.begin(), c.end());
}

// list like
template<typename Container, typename X>
enable_if_container_has_tag_t<Container, _detail::listlike_tag> erase(Container& c, const X& x) {
    c.remove(x);
}

template<typename Container, typename Predicate>
enable_if_container_has_tag_t<Container, _detail::listlike_tag> erase_if(Container& c, Predicate p) {
    c.remove_if(p);
}

template<typename Container, class Compare>
enable_if_container_has_tag_t<Container, _detail::listlike_tag> sort_comp(Container& c, Compare comp) {
    c.sort(comp);
}

template<typename Container>
enable_if_container_has_tag_t<Container, _detail::listlike_tag> sort(Container& c) {
    c.sort();
}

// maplike
template<typename Container, typename X>
enable_if_container_has_tag_t<Container, _detail::maplike_tag> erase(Container& c, const X& x) {
    c.erase(x);
}

template<typename Container, typename Predicate>
enable_if_container_has_tag_t<Container, _detail::maplike_tag> erase_if(Container& c, Predicate p) {
    for (auto i = c.begin(); i != c.end();) {
        if (p(*i)) {
            c.erase(i++);
        } else {
            ++i;
        }
    }
}

}}     // namespace ext::util
#endif // EXT_UTIL_CONTAINER_HELPER_HEADER
