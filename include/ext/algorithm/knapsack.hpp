// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_ALGORITHM_KNAPSACK_HEADER
#    define EXT_ALGORITHM_KNAPSACK_HEADER

#    include <iterator>
#    include <map>
#    include <stdexcept>
#    include <type_traits>

#    include <ext/math/math_types.hpp>
#include <iostream>

namespace ext {
namespace algorithm {


template<typename T = void>
struct item {
    std::size_t weight;
    std::size_t value;
    T* data = nullptr;

    std::ostream& operator<<(std::ostream& os) {
        return os << "(w: " << weight << " v: " << value << ")";
    }
};


template<typename T>
std::vector<item<T>> zero_or_one_knapsack(std::vector<item<T>>&& input, std::size_t max_weight) {

    ext::math::dynamic_matrix<std::size_t> matrix(input.size() + 1, max_weight + 1);
    matrix.init_all_to(0);

    // m[i,w] = m[i-1,w] if w > w_i else m[ i, w - w_i] + v_i
    for (std::size_t i = 1; i < input.size() + 1; i++) {
        auto const& item = input[i - 1]; // acess in input
        for (std::size_t j = 1; j <= max_weight; j++) {
            auto best_without_item = matrix.get(i - 1, j);
            if (item.weight <= j) {
                matrix.set(i, j, std::max(best_without_item, matrix.get(i - 1, j - item.weight) + item.value));
            } else {
                matrix.set(i, j, best_without_item);
            }
        }
    }

    // now find the output
    // pick if: m[i-1,w-w_i] + v_i > m[i-1,w]
    std::vector<item<T>> output;
    auto rest_weight = max_weight;
    for (std::size_t i = input.size(); i > 0; i--) {
        auto const& item = input[i - 1]; // access in input

        decltype(matrix)::value_type pick = 0;
        if (rest_weight >= item.weight) { // can only be picked if capacity fits
            pick = matrix.get(i - 1, rest_weight - item.weight) + item.value;
        }
        auto dont_pick = matrix.get(i - 1, rest_weight);

        if (pick > dont_pick) {
            rest_weight -= item.weight;
            output.push_back(item);
        }
    }

    return output;
}

} // namespace algorithm
namespace util {

template<typename T>
std::ostream& operator<<(std::ostream& os, ext::algorithm::item<T> const& item) {
    return os << item.weight << item.value;
}


} // namespace util
} // namespace ext

#endif
