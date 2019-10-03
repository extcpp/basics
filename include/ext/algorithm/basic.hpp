// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_ALGORITHM_BASIC_HEADER
#define EXT_ALGORITHM_BASIC_HEADER

#include "../meta/has_member.hpp"
#include <algorithm>
#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <vector>
namespace ext { namespace algorithm {

template<typename T, typename Predicate>
std::vector<T> partition_out(std::vector<T>& vec, Predicate pred) {
    auto end = vec.end();
    auto split_point = std::stable_partition(vec.begin(), end, pred);
    std::vector<T> out;
    out.reserve(static_cast<std::size_t>(std::distance(split_point, end)));
    std::copy(std::move_iterator(split_point), std::move_iterator(end), std::back_inserter(out));
    vec.erase(split_point, end);
    return out;
}

/*
template<typename T, typename Predicate>
std::vector<T> split_ub(std::vector<T>& vec, Predicate pred) {
    // this is unfortunatly ub as input and output overlap
    // self move assign does not need to be safe
    auto size = std::count_if(vec.begin(), vec.end(), pred );
    std::vector<T> out;
    out.reserve(vec.size() - static_cast<std::size_t>(size));
    auto res = std::partition_copy(
        std::move_iterator(vec.begin()), std::move_iterator(vec.end()), vec.begin(), std::back_inserter(out), pred);
    vec.erase(res.first, vec.end());
    return out;
}
*/

template<typename T, typename Predicate = std::less<>>
T const& min(T const& a, T const& b, T const& c, Predicate comp = Predicate()) {
    return std::min(a, std::min(b, c, comp), comp);
}

template<typename T, typename Predicate = std::less<>>
T const& max(T const& a, T const& b, T const& c, Predicate comp = Predicate()) {
    return std::max(std::max(a, b, comp), c, comp);
}

template<typename T>
T&& min(T&& x) {
    return std::forward<T>(x);
}

template<typename T1, typename T2, typename... Ts>
decltype(auto) min(T1&& x1, T2&& x2, Ts&&... xs) {
    return (x1 < x2) ? min(x1, std::forward<Ts>(xs)...) : min(x2, std::forward<Ts>(xs)...);
}

template<typename T>
T&& max(T&& x) {
    return std::forward<T>(x);
}

template<typename T1, typename T2, typename... Ts>
decltype(auto) max(T1&& x1, T2&& x2, Ts&&... xs) {
    return (x1 > x2) ? max(x1, std::forward<Ts>(xs)...) : max(x2, std::forward<Ts>(xs)...);
}

// count occurrences in container /////////////////////////////////////////
template<typename Iterator, typename Int = std::size_t>
auto count_occurrences(Iterator begin, Iterator end) {
    using Key = typename std::iterator_traits<Iterator>::value_type;
    std::map<Key, Int> result;
    for (auto it = begin; it != end; it++) {
        result[*it]++;
    }
    return result;
}

template<typename Container, typename Int = int>
auto count_occurrences(const Container& container) {
    using Iterator = decltype(std::begin(container));
    return count_occurrences<Iterator, Int>(std::begin(container), std::end(container));
}

// merge maps //////////////////////////////////////////////////////////////
// Items that are already present in the first map get replaced if
// the predicate evaluates to true
template<typename Map, typename Predicate = std::not_equal_to<>>
auto& merge_maps(Map& modify, const Map& add, Predicate predicate = Predicate()) {
    for (auto it = add.begin(); it != add.end(); it++) {
        auto found = modify.find(it->first);
        if (found != modify.end()) {
            if (predicate(found->second, it->second)) {
                modify[it->first] = it->second;
            }
        } else {
            modify[it->first] = it->second;
        }
    }
    return modify;
}

}}     // namespace ext::algorithm
#endif // EXT_ALGORITHM_BASIC_HEADER
