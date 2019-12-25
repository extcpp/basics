// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.14.8917&rep=rep1&type=pdf
// https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
#ifndef EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#define EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#include <iostream>
#include <iterator>
#include <vector>

namespace ext { namespace structures {

// detail
namespace detail {
// remove least set significant bit
template<typename T>
constexpr inline T remove_lsb(T number) {
    // number = (number & number-1);
    number -= (number & -number);
    return number;
}

// get index of next node
template<typename T>
constexpr inline T increase_lsb(T number) {
    // requires 2 complement
    if (number == T(0)) {
        number = T(1);
    }
    number += (number & -number);
    return number;
}
} // namespace detail

//Paper GetCumul
template<typename T, typename Iterator>
inline T bit_get_sum(Iterator begin, Iterator /*end*/, std::size_t index) {
    auto sum = *begin;
    while (index > 0) {
        sum += *(begin + index);
        index = detail::remove_lsb(index);
    }
    return sum;
}

//Paper PutValue
template<typename T, typename Iterator>
inline void bit_modify(Iterator begin, Iterator end, std::size_t index, T value) {
    if (index == 0) {
        *begin += value;
        return;
    }
    auto size = std::distance(begin, end);
    do {
        *(begin + index) += value;
        index = detail::increase_lsb(index);
    } while (index < std::size_t(size));
}

//Paper GetProb
template<typename T, typename Iterator>
inline T bit_get(Iterator begin, Iterator /*end*/, std::size_t index) {
    T rv = *(begin + index);
    if (index > std::size_t(0)) {
        auto search_index = index - 1; // first
        auto parent_index = detail::remove_lsb(index);
        while (search_index != parent_index) {
            rv -= *(begin + search_index);
            search_index = detail::remove_lsb(search_index);
        }
    }
    return rv;
}

template<typename T, typename Iterator>
inline void bit_set(Iterator begin, Iterator end, std::size_t index, T value) {
    auto current = bit_get<T>(begin, end, index);
    auto mod_value = value - current;
    bit_modify(begin, end, index, mod_value);
}

// template<typename T, typename Iterator>
// inline Iterator bit_find_cumulative(Iterator begin, Iterator end, T cum_value) {
//     std::size_t size = std::distance(begin, end);
//     std::size_t index = std::size_t(0);
//     T mask = T(1);
//     mask << size / 2;
//     if (cum_value > *begin) {
//     }
//     return begin + index;
// }

template< typename T>
class binary_index_tree {
public:
    binary_index_tree(std::size_t size) : _storage_size(size), _tree_size(size/2) {
        _storage.resize(size);
    }
private:
    std::vector<T> _storage;
    std::size_t _tree_size; // must be power of 2
    std::size_t _storage_size; // must greater equal tree_size
};

}}     // namespace ext::structures
#endif // EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
