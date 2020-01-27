// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>

#include <ext/algorithm/knapsack.hpp>
#include <ext/math/math_types.hpp>

std::vector<ext::algorithm::item<void>> const input1{
    {2, 1, nullptr},
    {3, 2, nullptr},
    {4, 5, nullptr},
    {5, 6, nullptr},
};
std::size_t capacity1 = 8;
std::size_t solution1 = 8;

std::vector<ext::algorithm::item<void>> const input2{
    {3, 4, nullptr}, // weight, value
    {4, 6, nullptr},
    {2, 2, nullptr},
    {1, 5, nullptr},
};
std::size_t capacity2 = 6;
std::size_t solution2 = 11;

template<typename T>
std::size_t accumulate(T const& container) {
    return std::accumulate(container.begin(), container.end(), 0UL, [](auto const& accumulation, auto const& item) {
        return accumulation + item.value;
    });
}


//#    include <ext/util/scoped_timer.hpp>
TEST(knapsack, zero_or_one) {
    ASSERT_EQ(accumulate(ext::algorithm::zero_or_one_knapsack(input1, capacity1)), solution1);
    ASSERT_EQ(accumulate(ext::algorithm::zero_or_one_knapsack(input2, capacity2)), solution2);
    ASSERT_EQ(accumulate(ext::algorithm::zero_or_one_knapsack2(input1, capacity1)), solution1);
    ASSERT_EQ(accumulate(ext::algorithm::zero_or_one_knapsack2(input2, capacity2)), solution2);
}
