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

// TODO generalize - Why didn't I write a variadic function?
template<typename T, typename Predicate = std::less<>>
T const& min(T const& a, T const& b, T const& c, Predicate comp = Predicate()) {
    return std::min(a, std::min(b, c, comp), comp);
}

template<typename T, typename Predicate = std::less<>>
T const& max(T const& a, T const& b, T const& c, Predicate comp = Predicate()) {
    return std::max(std::max(a, b, comp), c, comp);
}

// count occurrences in container /////////////////////////////////////////
template<typename Iterator, typename Int = std::size_t>
auto count_occurrences(Iterator begin, Iterator end) {
    // -> std::map<typename std::iterator_traits<Iterator>::value_type, Int>
    using Key = typename std::iterator_traits<Iterator>::value_type;
    std::map<Key, Int> result;
    for (auto it = begin; it != end; it++) {
        result[*it]++;
    }
    return result;
}

// TODO add enable_if or concept
template<typename Container, typename Int = int>
auto count_occurrences(const Container& container) {
    using Iterator = decltype(std::begin(container));
    return count_occurrences<Iterator, Int>(std::begin(container), std::end(container));
}

// merge maps //////////////////////////////////////////////////////////////
// items that are already in the map get replaced by a later insert if
// the predicate evaluates to true
template<typename Map, typename Predicate = std::less<>>
auto& merge_maps(Map& result, const Map& in, Predicate predicate = Predicate()) {
    for (auto it = in.begin(); it != in.end(); it++) {
        auto found = result.find(it->first);
        if (found != result.end()) {
            if (predicate(found->second, it->second)) {
                result[it->first] = it->second;
            }
        } else {
            result[it->first] = it->second;
        }
    }
    return result;
}

// looks like maps are given in a iterator range
template<typename Iterator,
         typename Predicate = std::less<>,
         typename = std::enable_if_t<ext::meta::is_input_iterator<Iterator>::value>>
auto merge_maps(Iterator begin, Iterator end, Predicate predicate = Predicate()) {
    using PairType = typename std::iterator_traits<Iterator>::value_type;
    std::map<typename PairType::T1, typename PairType::T2> result;
    for (auto it = begin; it != end; it++) {
        merge_maps(result, *it, predicate);
    }
    return result;
}
}}     // namespace ext::algorithm
#endif // EXT_ALGORITHM_BASIC_HEADER
