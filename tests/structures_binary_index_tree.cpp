#include <algorithm>
#include <ext/structures/binary_index_tree.hpp>
#include <ext/util/show.hpp>
#include <gtest/gtest.h>
#include <iostream>

using ext::util::operator<<;
namespace es = ext::structures;
namespace esd = ext::structures::detail;

TEST(structures_binary_index_tree, detail) {
    EXPECT_EQ(ext::structures::detail::remove_lsb(0), 0);
    EXPECT_EQ(ext::structures::detail::remove_lsb(1), 0);
    EXPECT_EQ(ext::structures::detail::remove_lsb(2), 0);
    EXPECT_EQ(ext::structures::detail::remove_lsb(3), 2);
    EXPECT_EQ(ext::structures::detail::remove_lsb(4), 0);
    EXPECT_EQ(ext::structures::detail::remove_lsb(5), 4);
    EXPECT_EQ(ext::structures::detail::remove_lsb(6), 4);
    EXPECT_EQ(ext::structures::detail::remove_lsb(7), 6);
    EXPECT_EQ(ext::structures::detail::remove_lsb(8), 0);
    EXPECT_EQ(ext::structures::detail::remove_lsb(9), 8);
    EXPECT_EQ(ext::structures::detail::remove_lsb(10), 8);
    EXPECT_EQ(ext::structures::detail::remove_lsb(11), 10);
    EXPECT_EQ(ext::structures::detail::remove_lsb(12), 8);
    EXPECT_EQ(ext::structures::detail::remove_lsb(13), 12);
    EXPECT_EQ(ext::structures::detail::remove_lsb(14), 12);
    EXPECT_EQ(ext::structures::detail::remove_lsb(15), 14);
    EXPECT_EQ(ext::structures::detail::remove_lsb(16), 0);

    EXPECT_EQ(ext::structures::detail::increase_lsb(0), 2);
    EXPECT_EQ(ext::structures::detail::increase_lsb(1), 2);
    EXPECT_EQ(ext::structures::detail::increase_lsb(2), 4);
    EXPECT_EQ(ext::structures::detail::increase_lsb(3), 4);
    EXPECT_EQ(ext::structures::detail::increase_lsb(4), 8);
    EXPECT_EQ(ext::structures::detail::increase_lsb(5), 6);
    EXPECT_EQ(ext::structures::detail::increase_lsb(6), 8);
    EXPECT_EQ(ext::structures::detail::increase_lsb(7), 8);
    EXPECT_EQ(ext::structures::detail::increase_lsb(8), 16);
    EXPECT_EQ(ext::structures::detail::increase_lsb(9), 10);
    EXPECT_EQ(ext::structures::detail::increase_lsb(10), 12);
    EXPECT_EQ(ext::structures::detail::increase_lsb(11), 12);
    EXPECT_EQ(ext::structures::detail::increase_lsb(12), 16);
    EXPECT_EQ(ext::structures::detail::increase_lsb(13), 14);
    EXPECT_EQ(ext::structures::detail::increase_lsb(14), 16);
    EXPECT_EQ(ext::structures::detail::increase_lsb(15), 16);
    EXPECT_EQ(ext::structures::detail::increase_lsb(16), 32);
}

template <typename Container>
std::vector<typename Container::value_type> get_sums(Container const& input) {
    std::vector<typename Container::value_type> prefix_sum;
    int sum = 0;
    for (auto element : input) {
        sum += element;
        prefix_sum.push_back(sum);
    }
    return prefix_sum;
}

template <typename Container>
std::vector<typename Container::value_type> bit_get_sums(Container const& input) {
    std::vector<typename Container::value_type> prefix_sum;
    for (std::size_t i = 0; i < input.size(); ++i) {
        prefix_sum.push_back(ext::structures::bit_get_sum<typename Container::value_type>(input.begin(), input.end(), i));
    }
    return prefix_sum;
}

template <typename Container>
std::vector<typename Container::value_type> bit_get_values(Container const& input) {
    std::vector<typename Container::value_type> values;
    for (std::size_t i = 0; i < input.size(); ++i) {
        values.push_back(ext::structures::bit_get<typename Container::value_type>(input.begin(), input.end(), i));
    }
    return values;
}

template <typename Input,  typename Bit>
void compare(Input const& input, Bit const& bit, std::string const& desc) {
    EXPECT_EQ(input, bit_get_values(bit)) << desc;

    auto prefix_sum = get_sums(input);
    auto bit_prefix_sum = bit_get_sums(bit);
    ASSERT_EQ(prefix_sum, bit_prefix_sum) << desc;
}

TEST(structures_binary_index_tree, functions) {
    // clang-format off
    std::vector<int> input {1, 3, 4, 8,
                            5, 5, 3, 2,
                            1, 2, 3, 2,
                            5, 7, 3, 5};
    // clang-format on
    std::array<int, 16> bit; // binary index tree
    ASSERT_EQ(bit.size(), bit.size());
    bit.fill(0);

    for (auto it = input.begin(); it != input.end(); it++) {
        ext::structures::bit_modify(bit.begin(), bit.end(), static_cast<std::size_t>(std::distance(input.begin(), it)), *it);
    }
    compare(input, bit, "initial");

    auto add_at_value = [&input, &bit](std::size_t index, auto value) {
        ext::structures::bit_modify(bit.begin(), bit.end(), index, value);
        input[index] += value;
    };

    auto set_at_value = [&input, &bit](std::size_t index, auto value) {
        ext::structures::bit_set(bit.begin(), bit.end(), index, value);
        input[index] = value;
    };


    add_at_value(2,3);
    compare(input, bit, "1st modification");

    add_at_value(0,3);
    compare(input, bit, "2nd modification");

    set_at_value(8,23);
    compare(input, bit, "3rd modification");

    set_at_value(8,2);
    compare(input, bit, "4th modification");

    set_at_value(0,0);
    compare(input, bit, "5th modification");

}
