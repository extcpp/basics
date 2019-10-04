#include <gtest/gtest.h>

#include <ext/algorithm/basic.hpp>
#include <set>

TEST(algorithm_basic, stable_partition_out) {
    {
        std::vector<int> first = {1, 6, 2, 7, 3, 8, 4, 9, 5};
        auto second = ext::algorithm::stable_partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{1, 2, 3, 4, 5}));
        EXPECT_EQ(second, (std::vector<int>{6, 7, 8, 9}));
    }

    {
        std::vector<int> first = {1, 6, 2, 7, 3, 8, 4, 9, 5};
        auto second = ext::algorithm::stable_partition_out(first, [](int const& value) { return value > 5; });
        EXPECT_EQ(first, (std::vector<int>{6, 7, 8, 9}));
        EXPECT_EQ(second, (std::vector<int>{1, 2, 3, 4, 5}));
        EXPECT_EQ(second.capacity(), second.size());
    }

    {
        std::vector<int> first = {};
        auto second = ext::algorithm::stable_partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{}));
        EXPECT_EQ(second, (std::vector<int>{}));
    }

    {
        std::vector<int> first = {5};
        auto second = ext::algorithm::stable_partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(first, (std::vector<int>{5}));
        EXPECT_EQ(second, (std::vector<int>{}));
    }
}

TEST(algorithm_basic, partition_out) {
    auto to_set = [](std::vector<int> const& vec) { return std::set<int>(vec.begin(), vec.end()); };

    {
        std::vector<int> first = {1, 6, 2, 7, 3, 8, 4, 9, 5};
        auto second = ext::algorithm::partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(to_set(first), to_set(std::vector<int>{1, 2, 3, 4, 5}));
        EXPECT_EQ(to_set(second), to_set(std::vector<int>{6, 7, 8, 9}));
    }

    {
        std::vector<int> first = {1, 6, 2, 7, 3, 8, 4, 9, 5};
        auto second = ext::algorithm::partition_out(first, [](int const& value) { return value > 5; });
        EXPECT_EQ(to_set(first), to_set(std::vector<int>{6, 7, 8, 9}));
        EXPECT_EQ(to_set(second), to_set(std::vector<int>{1, 2, 3, 4, 5}));
        EXPECT_EQ(second.capacity(), second.size());
    }

    {
        std::vector<int> first = {};
        auto second = ext::algorithm::partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(to_set(first), to_set(std::vector<int>{}));
        EXPECT_EQ(to_set(second), to_set(std::vector<int>{}));
    }

    {
        std::vector<int> first = {5};
        auto second = ext::algorithm::partition_out(first, [](int const& value) { return value <= 5; });
        EXPECT_EQ(to_set(first), to_set(std::vector<int>{5}));
        EXPECT_EQ(to_set(second), to_set(std::vector<int>{}));
    }
}

TEST(algorithm_basic, min_max) {
    EXPECT_EQ(1, ext::algorithm::min(3, 2, 1));
    EXPECT_EQ(1, ext::algorithm::min(1, 2, 3));
    EXPECT_EQ(3, ext::algorithm::max(3, 2, 1));
    EXPECT_EQ(3, ext::algorithm::max(1, 2, 3));

    EXPECT_EQ(1, ext::algorithm::min(5, 4, 3, 2, 1));
    EXPECT_EQ(1, ext::algorithm::min(1, 2, 3, 4, 5));
    EXPECT_EQ(5, ext::algorithm::max(5, 4, 3, 2, 1));
    EXPECT_EQ(5, ext::algorithm::max(1, 2, 3, 4, 5));
}

TEST(algorithm_basic, count_occurrences) {
    std::vector<int> in{2, 1, 3, 3, 2, 3};
    auto out = ext::algorithm::count_occurrences(in);

    EXPECT_EQ(1, out[1]);
    EXPECT_EQ(2, out[2]);
    EXPECT_EQ(3, out[3]);
}

TEST(algorithm_basic, merge_maps) {
    std::map<int, int> modify{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    std::map<int, int> add{{2, 20}, {5, 50}, {6, 60}};
    std::map<int, int> expected{{1, 1}, {2, 20}, {3, 3}, {4, 4}, {5, 50}, {6, 60}};

    ext::algorithm::merge_maps(modify, add);
    EXPECT_EQ(modify, expected);
}
