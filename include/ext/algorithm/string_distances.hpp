// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_ALGORITHM_STRING_DISTANCES_HEADER
#    define EXT_ALGORITHM_STRING_DISTANCES_HEADER

#    include <ext/algorithm/basic.hpp>
#    include <string>
#    include <vector>

namespace ext { namespace algorithm { namespace distances {

// For Unicode we would require lib ICU

// simple length
template<typename T>
std::size_t length(const T& first, const T& second) {
    // needs to be cast to int or subtraction will have a problem!
    return std::max(first.size(), second.size()) - std::min(first.size(), second.size());
}

// edit distance fast implementation
template<typename T>
std::size_t edit_fast(const T& first, const T& second) {
    const std::size_t items_per_col = first.size();
    const std::size_t number_of_cols = second.size();

    std::vector<std::size_t> previous_col(items_per_col + 1);
    std::vector<std::size_t> current_col(items_per_col + 1);

    for (std::size_t i = 0; i <= items_per_col; i++) {
        previous_col[i] = i;
    }

    for (std::size_t j = 1; j <= number_of_cols; j++) {
        current_col[0] = j;
        for (std::size_t i = 1; i <= items_per_col; i++) {
            current_col[i] = min(current_col[i - 1] + 1,                                          // delete
                                 previous_col[i] + 1,                                             // insert
                                 previous_col[i - 1] + (first.at(i - 1) == second.at(j - 1) ? 0 : // equal
                                                            1                                     // replace
                                                        ));
        }
        current_col.swap(previous_col);
    }
    return previous_col[items_per_col];
} // edit_fast

template<typename T>
std::size_t edit_matrix(const T& first, const T& second) {
    // requires more memory
    std::size_t items_per_col = first.size();
    std::size_t number_of_cols = second.size();

    std::vector<std::vector<std::size_t>> matrix(items_per_col + 1, std::vector<std::size_t>(number_of_cols + 1));

    for (std::size_t i = 0; i <= items_per_col; ++i) {
        matrix[i][0] = i;
    }

    for (std::size_t j = 0; j <= number_of_cols; ++j) {
        matrix[0][j] = j;
    }

    for (std::size_t j = 1; j <= number_of_cols; ++j) {
        for (std::size_t i = 1; i <= items_per_col; ++i) {
            matrix[i][j] = min(matrix[i - 1][j] + 1,
                               matrix[i][j - 1] + 1,
                               matrix[i - 1][j - 1] + (first[i - 1] == second[j - 1] ? 0 : 1));
        }
    }
    return matrix[items_per_col][number_of_cols];
} // edit_matrix

template<typename T>
std::size_t edit(const T& first, const T& second) {
    return edit_matrix(first, second);
}
}}}    // namespace ext::algorithm::distances
#endif // EXT_ALGORITHM_STRING_DISTANCES_HEADER
