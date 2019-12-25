#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <array>

#define EXT_STRUCTURES_BINARY_INDEX_TREE_TEST
#include <ext/structures/binary_index_tree.hpp>
#include <ext/util/show.hpp>

TEST(structures_binary_index_tree, detail) {
    EXPECT_EQ(ext::structures::detail::parent_interrogation(0), 0);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(1), 0);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(2), 0);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(3), 2);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(4), 0);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(5), 4);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(6), 4);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(7), 6);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(8), 0);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(9), 8);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(10), 8);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(11), 10);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(12), 8);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(13), 12);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(14), 12);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(15), 14);
    EXPECT_EQ(ext::structures::detail::parent_interrogation(16), 0);

    EXPECT_EQ(ext::structures::detail::parent_update(1), 2);
    EXPECT_EQ(ext::structures::detail::parent_update(2), 4);
    EXPECT_EQ(ext::structures::detail::parent_update(3), 4);
    EXPECT_EQ(ext::structures::detail::parent_update(4), 8);
    EXPECT_EQ(ext::structures::detail::parent_update(5), 6);
    EXPECT_EQ(ext::structures::detail::parent_update(6), 8);
    EXPECT_EQ(ext::structures::detail::parent_update(7), 8);
    EXPECT_EQ(ext::structures::detail::parent_update(8), 16);
    EXPECT_EQ(ext::structures::detail::parent_update(9), 10);
    EXPECT_EQ(ext::structures::detail::parent_update(10), 12);
    EXPECT_EQ(ext::structures::detail::parent_update(11), 12);
    EXPECT_EQ(ext::structures::detail::parent_update(12), 16);
    EXPECT_EQ(ext::structures::detail::parent_update(13), 14);
    EXPECT_EQ(ext::structures::detail::parent_update(14), 16);
    EXPECT_EQ(ext::structures::detail::parent_update(15), 16);
    EXPECT_EQ(ext::structures::detail::parent_update(16), 32);
}

template<typename Container>
std::vector<typename Container::value_type> get_sums(Container const& input) {
    std::vector<typename Container::value_type> prefix_sum;
    int sum = 0;
    for (auto element : input) {
        sum += element;
        prefix_sum.push_back(sum);
    }
    return prefix_sum;
}

template<typename Container>
std::vector<typename Container::value_type> bit_get_sums(Container const& input) {
    std::vector<typename Container::value_type> prefix_sum;
    for (std::size_t i = 0; i < input.size(); ++i) {
        prefix_sum.push_back(
            ext::structures::bit_get_sum<typename Container::value_type>(input.begin(), input.end(), i));
    }
    return prefix_sum;
}

template<typename Container>
std::vector<typename Container::value_type> bit_get_values(Container const& input) {
    std::vector<typename Container::value_type> values;
    for (std::size_t i = 0; i < input.size(); ++i) {
        values.push_back(ext::structures::bit_get<typename Container::value_type>(input.begin(), input.end(), i));
    }
    return values;
}

template<typename Input, typename BitArr, typename Bit>
void compare(Input const& input, BitArr const& bit_arr, Bit const& bit, std::string const& desc) {
    ASSERT_EQ(input, bit_get_values(bit_arr)) << desc;
    ASSERT_EQ(input, bit.value_vec()) << desc;

    auto prefix_sum = get_sums(input);
    auto bit_prefix_sum = bit_get_sums(bit_arr);
    ASSERT_EQ(prefix_sum, bit_prefix_sum) << desc;
    ASSERT_EQ(prefix_sum, bit.sum_vec()) << desc;
}

TEST(structures_binary_index_tree, functions) {
    // clang-format off
    std::vector<int> input {1, 3, 4, 8,
                            5, 5, 3, 2,
                            1, 2, 3, 2,
                            5, 7, 3, 5};
    // clang-format on
    std::array<int, 16> bit_arr; // binary index tree
    ASSERT_EQ(bit_arr.size(), bit_arr.size());
    bit_arr.fill(0);

    ext::structures::binary_index_tree<int> bit(5);
    ASSERT_EQ(bit.size(), 8);


    for (auto it = input.begin(); it != input.end(); it++) {
        auto index = static_cast<std::size_t>(std::distance(input.begin(), it));
        ext::structures::bit_modify(bit_arr.begin(), bit_arr.end(), index, *it);
        bit.add(index, *it);
    }
    compare(input, bit_arr, bit, "initial");

    auto add_at_value = [&input, &bit_arr, &bit](std::size_t index, auto value) {
        ext::structures::bit_modify(bit_arr.begin(), bit_arr.end(), index, value);
        bit.add(index, value);
        input[index] += value;
    };

    auto set_at_value = [&input, &bit_arr, &bit](std::size_t index, auto value) {
        ext::structures::bit_set(bit_arr.begin(), bit_arr.end(), index, value);
        bit.set(index, value);
        input[index] = value;
    };


    add_at_value(2, 3);
    compare(input, bit_arr, bit, "1st modification");

    add_at_value(0, 3);
    compare(input, bit_arr, bit, "2nd modification");

    set_at_value(8, 23);
    compare(input, bit_arr, bit, "3rd modification");

    set_at_value(8, 2);
    compare(input, bit_arr, bit, "4th modification");

    set_at_value(0, 0);
    compare(input, bit_arr, bit, "5th modification");

    bit.reset();
    ASSERT_EQ(bit.size(), 8);
}
