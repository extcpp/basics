// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#define EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#include <iostream>
#include <iterator>

namespace ext { namespace structures {

// detail
namespace detail {
// remove least set significant bit
template<typename T>
inline T remove_lsb(T& number) {
    // number = (number & number-1);
    number -= (number & -number);
    return number;
}

// get index of next node
template<typename T>
inline T increase_lsb(T& number) {
    // requires 2 complement
    if (number == T(0)) {
        number = T(1);
    }
    number += (number & -number);
    return number;
}
} // namespace detail

template<typename T, typename Iterator, typename Index>
inline T bit_get_cumulative(Iterator begin, Iterator /*end*/, Index index) {
    auto sum = *begin;
    while (index > 0) {
        sum += *(begin + index);
        detail::remove_lsb(index);
    }
    return sum;
}

template<typename T, typename Iterator, typename Index>
inline void bit_modify(Iterator begin, Iterator end, Index index, T value) {
    if (index == Index(0)) {
        *begin = value;
        return;
    }
    auto size = std::distance(begin, end);
    do {
        *(begin + index) += value;
        detail::increase_lsb(index);
    } while (index < Index(size));
    // return value; // is there something we can return
}

template<typename T, typename Iterator, typename Index>
inline T bit_get(Iterator begin, Iterator /*end*/, Index index) {
    T rv = *(begin + index);
    if (index > Index(0)) {
        auto search_index = index - 1; // first
        auto parent_index = detail::remove_lsb(index);
        while (search_index != parent_index) {
            rv -= *(begin + search_index);
            detail::remove_lsb(search_index);
        }
    }
    return rv;
}

template<typename T, typename Iterator, typename Index>
inline void bit_set(Iterator begin, Iterator end, Index index, T value) {
    auto current = bit_get<T>(begin, end, index);
    auto mod_value = value - current;
    bit_modify(begin, end, index, mod_value);
}

template<typename T, typename Iterator, typename Index>
inline Iterator bit_find_cumulative(Iterator begin, Iterator end, T cum_value) {
    Index size = std::distance(begin, end);
    Index index = Index(0);
    T mask = T(1);
    mask << size / 2;
    if (cum_value > *begin) {
    }
    return begin + index;
}
}}     // namespace ext::structures
#endif // EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
