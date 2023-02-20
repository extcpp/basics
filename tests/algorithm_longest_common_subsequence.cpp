// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/algorithm/longest_common_subsequence.hpp>
#include <gtest/gtest.h>

namespace ext { namespace algorithm {
class algorithm_longest_common_subsequence : public ::testing::Test {
    protected:
    typedef std::vector<std::pair<std::string, std::string>> test_data_type;
    test_data_type test_data;
    algorithm_longest_common_subsequence() {
        test_data = {
            {"MZJAWXU", "MXJYAUZ"}, {"1234567890", "67890"}, {"jan", "mann"}, {"loster", "lopster"}, {"hallo", "hallo"}};
    }

    virtual ~algorithm_longest_common_subsequence() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

    void test_function(std::vector<std::size_t> lenghts, std::vector<std::string> words) {
        for (std::size_t i = 0; i < lenghts.size(); ++i) {
            auto const& test_string_pair = test_data[i];
            auto matrix = lcs_solve(test_string_pair.first, test_string_pair.second);
            auto lenght = lsc_get_length(matrix);
            auto word = lcs_get_sequence(matrix, test_string_pair.first);

            EXPECT_TRUE(lenghts[i] == lenght && words[i] == word)
                << "\n    index: "  << i
                << "\n    input: '" << test_string_pair.first << "' and '" << test_string_pair.second << "'"
                << "\n   result: "  << lenght << " -- " <<  word
                << "\n expected: "  << lenghts[i] << " -- " << words[i]
                // << "\n" << ext::util::pretty::fmt(matrix) // confuses github's compiler
                << "\n\n";
        }
    }
};

TEST_F(algorithm_longest_common_subsequence, length) {
    test_function({4, 5, 2, 6, 5}, {"MJAU", "67890", "an", "loster", "hallo"});
}

}} // namespace ext::algorithm::distances

