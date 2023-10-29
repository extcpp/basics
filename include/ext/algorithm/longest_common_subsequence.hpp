// Copyright - 2023 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_ALGORITHM_LONGEST_COMMON_SUBSEQUENCE_HEADER
#define EXT_ALGORITHM_LONGEST_COMMON_SUBSEQUENCE_HEADER

#include <algorithm>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

#include <ext/util/pretty.hpp>

namespace ext { namespace algorithm {

using lcs_matrix = std::vector<std::vector<std::size_t>>; // m x n
// a: MXJYAUZ
// b: MZJAWXU
//
// x[i][j] n  with j
//           0  1  2  3  4  5  6  7
//           ep b0 b1 b2 b3 b4 b5 b6
//           -  M  Z  J  A  W  X  U
// m  0 ep - 0  0  0  0  0  0  0  0
//    1 a0 X 0  0  0  0  0  0  1  1
// w  2 a1 M 0  1  1  1  1  1  1  1
// i  3 a2 J 0  1  1  2  2  2  2  2
// t  4 a3 Y 0  1  1  2  2  2  2  2
// h  5 a4 A 0  1  1  2  3  3  3  3
//    6 a5 U 0  1  1  2  3  3  3  4
// i  7 a6 Z 0  1  1  2  3  3  3  4
//
// seq: MJAU

template <typename Container>
lcs_matrix lcs_solve(Container const& a, Container const& b) {
    auto m = a.size();
    auto n = b.size();
    lcs_matrix rv(m + 1, std::vector<std::size_t>(n + 1, 0));
    for (std::size_t i = 1; i < m + 1; ++i) {     // iterates over a
        for (std::size_t j = 1; j < n + 1; ++j) { // iterates over b
            if (a[i - 1] == b[j - 1]) {           // -1 because w have the eps
                rv[i][j] = rv[i - 1][j - 1] + 1;  // if a char matches
            } else {
                rv[i][j] = std::max({rv[i - 1][j], rv[i][j - 1]});
            }
        }
    }
    return rv;
}

inline std::size_t lsc_get_length(lcs_matrix const& m) {
    return m.back().back();
}

template <typename Container>
Container lcs_get_sequence(lcs_matrix const& m, Container const& a) {
    Container rv;
    std::size_t i = m.size() - 1;
    std::size_t j = m.front().size() - 1;
    std::size_t value = m[i][j];

    while (value != 0) {
        if (m[i - 1][j] == value) {
            --i;
        } else if (m[i][j - 1] == value) {
            --j;
        } else {
            rv.push_back(a[i - 1]);
            value = m[--i][--j];
        }
    }
    std::reverse(rv.begin(), rv.end());
    return rv;
}

}} // namespace ext::algorithm

#endif // EXT_ALGORITHM_LONGEST_COMMON_SUBSEQUENCE_HEADER
