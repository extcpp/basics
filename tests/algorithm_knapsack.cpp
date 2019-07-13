#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>

#include <ext/algorithm/knapsack.hpp>
#include <ext/math/math_types.hpp>


TEST(knapsack, zero_or_one) {
    std::vector<ext::algorithm::item<void>> input{
        {2, 1, nullptr},
        {3, 2, nullptr},
        {4, 5, nullptr},
        {5, 6, nullptr},
    };

    auto out = ext::algorithm::zero_or_one_knapsack(std::move(input), 8);
    auto result = std::accumulate(out.begin(), out.end(), 0UL, [](auto const& accumulation , auto const& item) {
        return accumulation + item.value;
    });

    ASSERT_EQ(result, 8);
}
