#include <ext/algorithm/string_distances.hpp>
#include <gtest/gtest.h>

namespace ext { namespace algorithm { namespace distances {
class string_distance_fixture : public ::testing::Test {
    protected:
    typedef std::vector<std::pair<std::string, std::string>> test_data_type;
    test_data_type test_data;
    string_distance_fixture() {
        test_data = {
            {"ulaula", "ohlala"}, {"1234567890", "67890"}, {"jan", "mann"}, {"loster", "lopster"}, {"hallo", "hallo"}};
    }

    virtual ~string_distance_fixture() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

    void test_function(std::size_t distance_fun(const std::string&, const std::string&),
                       std::vector<std::size_t> result) {
        for (std::size_t i = 0; i < result.size(); ++i) {
            std::size_t j;
            EXPECT_EQ(result[i], j = distance_fun(test_data[i].first, test_data[i].second))
                << "\n"
                << "   index: " << i << std::endl
                << "   input: '" << test_data[i].first << "' and '" << test_data[i].second << "'\n"
                << "  result: " << j << std::endl
                << "expected: " << result[i] << "\n\n";
        }
    }
};

TEST_F(string_distance_fixture, length) {
    test_function(length, {0, 5, 1, 1, 0});
}

TEST_F(string_distance_fixture, edit_matrix) {
    test_function(edit_matrix, {3, 5, 2, 1, 0});
}

TEST_F(string_distance_fixture, edit_fast) {
    test_function(edit_fast, {3, 5, 2, 1, 0});
}
}}} // namespace ext::algorithm::distances
