// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_ALGORITHM_KNAPSACK_HEADER
#define EXT_ALGORITHM_KNAPSACK_HEADER

#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>

#include <ext/macros/assert.hpp>
#include <ext/math/math_types.hpp>

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
inline std::vector<item<T>> zero_or_one_knapsack(std::vector<item<T>> const& input, std::size_t max_weight) {

    ext::math::dynamic_matrix<std::size_t> matrix(input.size() + 1, max_weight + 1);
    matrix.init_all_to(0);

    // m[i,w] = m[i-1,w] if w > w_i else m[ i, w - w_i] + v_i
    for (std::size_t i = 1; i < input.size() + 1; i++) {
        auto const& item = input[i - 1]; // access in input
        for (std::size_t w = 1; w <= max_weight; w++) {
            auto best_without_item = matrix.get(i - 1, w);
            if (item.weight <= w) {
                matrix.set(i, w, std::max(best_without_item, matrix.get(i - 1, w - item.weight) + item.value));
            } else {
                matrix.set(i, w, best_without_item);
            }
        }
    }

    // now find the output
    // pick if: m[i-1,w-w_i] + v_i > m[i-1,w]
    std::vector<item<T>> output;
    auto rest_weight = max_weight;
    for (std::size_t i = input.size(); i > 0; i--) {
        auto const& item = input[i - 1]; // access in input

        decltype(matrix)::value_type if_picked_value = 0;
        if (rest_weight >= item.weight) { // can only be picked if capacity fits
            if_picked_value = matrix.get(i - 1, rest_weight - item.weight) + item.value;
        }
        auto if_not_picked_value = matrix.get(i - 1, rest_weight);

        if (if_picked_value > if_not_picked_value) {
            rest_weight -= item.weight;
            output.push_back(item);
        }
    }

    return output;
}

template<typename T>
inline std::vector<item<T>> zero_or_one_knapsack2(std::vector<item<T>> const& input, std::size_t max_weight) {
    // matrix uses smaller type - but we need an extra array
    // TODO - implement with bitfield matrix?
    ext::math::dynamic_matrix<std::uint8_t> matrix(input.size() + 1, max_weight + 1);
    matrix.init_all_to(0);

    std::vector<std::size_t> max_for_weight(max_weight + 1);
    std::fill(max_for_weight.begin(), max_for_weight.end(), 0);

    for (std::uint64_t i = 1; i < input.size() + 1; i++) {
        auto const& item = input[i - 1];
        for (std::size_t w = max_weight; w > 0; w--) {
            EXT_ASSERT(0 <= w && w <= max_weight);

            std::size_t s = 0;
            if (w >= item.weight) {
                s = max_for_weight[w - item.weight] + item.value;
            }

            if (s > max_for_weight[w]) {
                max_for_weight[w] = s;
                matrix.set(i, w, 1);
            } else {
                matrix.set(i, w, 0);
            }
        }
    }

    std::vector<item<T>> output;
    std::size_t rest_weight = max_weight;

    for (std::size_t i = input.size(); i > 0; i--) {
        auto const& item = input[i - 1]; // access in input
        bool was_picked = matrix.get(i, rest_weight);
        if (was_picked) {
            rest_weight -= item.weight;
            output.push_back(item);
        }
    }
    return output;
}

} // namespace algorithm
namespace util {

template<typename T>
inline std::ostream& operator<<(std::ostream& os, ext::algorithm::item<T> const& item) {
    return os << item.weight << item.value;
}

} // namespace util
} // namespace ext

#endif // EXT_ALGORITHM_KNAPSACK_HEADER
