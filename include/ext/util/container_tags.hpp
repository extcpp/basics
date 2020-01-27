// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
// Idea and code from STL
// http://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-3-of-n
#ifndef EXT_UTIL_CONTAINER_TAGS_HEADER
#define EXT_UTIL_CONTAINER_TAGS_HEADER
#include <ext/meta/basic.hpp>

// https://blog.magnum.graphics/backstage/forward-declaring-stl-container-types/
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace ext { namespace util {
namespace _detail {

// possilbe categorys for containers
struct arraylike_tag {};
struct vectorlike_tag {};
struct listlike_tag {};
struct maplike_tag {};


// trait that sets the category ///////////////////////////////////////////////
template<typename C>
struct container_traits {};

// array like
template<typename T, std::size_t N>
struct container_traits<std::array<T, N>> {
    using category = arraylike_tag;
};

// vector like
template<typename T, typename A>
struct container_traits<std::vector<T, A>> {
    using category = vectorlike_tag;
};

template<typename T, typename A>
struct container_traits<std::deque<T, A>> {
    using category = vectorlike_tag;
};

// list like
template<typename T, typename A>
struct container_traits<std::list<T, A>> {
    using category = listlike_tag;
};

template<typename T, typename A>
struct container_traits<std::forward_list<T, A>> {
    using category = listlike_tag;
};

// maplike
template<typename T, typename C, typename A>
struct container_traits<std::set<T, C, A>> {
    using category = maplike_tag;
};

template<typename T, typename C, typename A>
struct container_traits<std::multiset<T, C, A>> {
    using category = maplike_tag;
};

template<typename T, typename C, typename A>
struct container_traits<std::unordered_set<T, C, A>> {
    using category = maplike_tag;
};

template<typename T, typename C, typename A>
struct container_traits<std::unordered_multiset<T, C, A>> {
    using category = maplike_tag;
};

template<typename K, typename V, typename C, typename A>
struct container_traits<std::map<K, V, C, A>> {
    using category = maplike_tag;
};

template<typename K, typename V, typename C, typename A>
struct container_traits<std::multimap<K, V, C, A>> {
    using category = maplike_tag;
};

template<typename K, typename V, typename C, typename A>
struct container_traits<std::unordered_map<K, V, C, A>> {
    using category = maplike_tag;
};

template<typename K, typename V, typename C, typename A>
struct container_traits<std::unordered_multimap<K, V, C, A>> {
    using category = maplike_tag;
};

} // namespace _detail

template<typename Container, typename Tag>
constexpr bool container_has_tag_v =
    std::is_same_v<typename _detail::container_traits<meta::remove_cv_ref_t<Container>>::category, Tag>;

template<typename Container, typename Tag, typename T = void>
using enable_if_container_has_tag_t = std::enable_if_t<container_has_tag_v<Container, Tag>, T>;


}} // namespace ext::util

#endif // EXT_UTIL_CONTAINER_TAGS_HEADER
