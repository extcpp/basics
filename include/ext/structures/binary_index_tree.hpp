// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.14.8917&rep=rep1&type=pdf
// https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
#ifndef EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#define EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
#include <iostream>
#include <iterator>
#include <vector>

#include <ext/macros/assert.hpp>
#include <ext/memory/align.hpp>

namespace ext { namespace structures {

namespace detail {
// remove least set significant bit
template<typename T>
constexpr inline T remove_lsb(T number) {
    static_assert(-1 == ~0, "not 2's complement");
    number -= (number & -number);
    return number;
}

// get index of next node
template<typename T>
constexpr inline T increase_lsb(T number) {
    static_assert(-1 == ~0, "not 2's complement");
    if (number == T(0)) {
        number = T(1);
    }
    number += (number & -number);
    return number;
}
} // namespace detail

// Paper GetCumul
template<typename T, typename Iterator>
inline T bit_get_sum(Iterator begin, Iterator /*end*/, std::size_t index) {
    auto sum = *begin;
    while (index > 0) {
        sum += *(begin + index);
        index = detail::remove_lsb(index);
    }
    return sum;
}

// Paper PutValue
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

// Paper GetProb
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

template<typename T>
class binary_index_tree {
    public:
    binary_index_tree(std::size_t size = 8) {
        std::size_t new_size = size;
        // check if we git a power of 2 if not take the next bigger one
        if (!ext::memory::is_alignment(size)) {
            std::size_t msb = 0;
            while (size != 0) {
                size = size / 2;
                msb++;
            }
            new_size = (1 << msb);
        }

        _storage.resize(new_size);
        _tree_size = new_size;
        _start_size = new_size;
    }

    T get(std::size_t index) const {
        EXT_ASSERT(index < _tree_size);
        return bit_get<T>(_storage.begin(), _storage.end(), index);
    }

    T get_sum(std::size_t index) const {
        EXT_ASSERT(index < _tree_size);
        return bit_get_sum<T>(_storage.begin(), _storage.end(), index);
    }

    void set(std::size_t index, T value) {
        grow_fill(index);
        bit_set<T>(_storage.begin(), _storage.end(), index, value);
    }

    void add(std::size_t index, T value) {
        grow_fill(index);
        bit_modify(_storage.begin(), _storage.end(), index, value);
    }

    auto size() {
        return _storage.size();
    }
    void reset() {
        _storage.clear();
        _storage.resize(_start_size);
        _tree_size = _start_size;
    }

#ifdef EXT_STRUCTURES_BINARY_INDEX_TREE_TEST
    std::vector<T> value_vec() const {
        std::vector<T> result;
        for (std::size_t i = 0; i < _tree_size; i++) {
            result.push_back(get(i));
        }
        return result;
    }
    std::vector<T> sum_vec() const {
        std::vector<T> result;
        for (std::size_t i = 0; i < _tree_size; i++) {
            result.push_back(get_sum(i));
        }
        return result;
    }

#endif
    private:
    void grow_fill(std::size_t index) {
        if (index >= _tree_size) {
            EXT_ASSERT(ext::memory::is_alignment(_tree_size));
            auto new_size = _tree_size;
            while (new_size <= index) {
                new_size = new_size * 2;
            }
            _storage.resize(new_size);
            auto old_size = _tree_size;
            _tree_size = new_size;
            set(old_size, T(0)); // fix tree
        }
        EXT_ASSERT(index < _storage.size());
    }

    std::vector<T> _storage;
    std::size_t _tree_size = 0; // must be power of 2
    std::size_t _start_size = 0;
};

}}     // namespace ext::structures
#endif // EXT_STRUCTURES_BINARY_INDEX_TREE_HEADER
