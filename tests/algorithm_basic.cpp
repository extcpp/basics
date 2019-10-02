#include <gtest/gtest.h>

#include <ext/algorithm/basic.hpp>

TEST(algorithm_basic, split) {
    {
        std::vector<int> first = {1, 6, 2, 7, 3, 8, 4, 9, 5};
        auto second = ext::algorithm::split(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{1, 2, 3, 4, 5}));
        EXPECT_EQ(second, (std::vector<int>{6, 7, 8, 9}));
    }

    {
        std::vector<int> first = {};
        auto second = ext::algorithm::split(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{}));
        EXPECT_EQ(second, (std::vector<int>{}));
    }

    {
        std::vector<int> first = {5};
        auto second = ext::algorithm::split(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{5}));
        EXPECT_EQ(second, (std::vector<int>{}));
    }
}

TEST(algorithm_basic, count_occurrences) {
    std::vector<int> in { 2, 1, 3, 3, 2, 3 } ;
    auto out = ext::algorithm::count_occurrences(in.begin(), in.end());

    EXPECT_EQ(1, out[1]);
    EXPECT_EQ(2, out[2]);
    EXPECT_EQ(3, out[3]);
}
