// Copyright - 2023 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/algorithm/longest_common_subsequence.hpp>
#include <gtest/gtest.h>

namespace ext { namespace algorithm {
class algorithm_longest_common_subsequence : public ::testing::Test {
    protected:
    void test_function(std::pair<std::string, std::string> sequences, std::size_t expected_lenght, std::string expected_common) {
            auto const& test_string_pair = sequences;
            auto matrix = lcs_solve(test_string_pair.first, test_string_pair.second);
            auto lenght = lsc_get_length(matrix);
            auto common = lcs_get_sequence(matrix, test_string_pair.first);

        EXPECT_TRUE(expected_lenght == lenght && expected_common == common)
            << "\n    input: '" << test_string_pair.first << "' and '" << test_string_pair.second << "'"
            << "\n   result: " << lenght << " -- " << common << "\n expected: " << expected_lenght << " -- "
            << expected_common << "\n"
#ifndef CLANG_ANALYZE
            << ext::util::pretty::fmt(matrix) << "\n\n";
#else
            << "\n";
#endif
    }
};

TEST_F(algorithm_longest_common_subsequence, length) {
    test_function({"MZJAWXU", "MXJYAUZ"}, 4, "MJAU");
    test_function({"1234567890", "67890"}, 5, "67890");
    test_function({"jan", "mann"}, 2, "an");
    test_function({"loster", "lopster"}, 6,"loster");
    test_function({"hallo", "hallo"}, 5, "hallo");
}

}} // namespace ext::algorithm::distances
