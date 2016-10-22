#include <gtest/gtest.h>
#include <obi/algorithm/string_distances.hpp>

namespace obi::algorithm::distances {
    class string_distance_fixture : public ::testing::Test {
        protected:
            typedef std::vector<std::pair<std::string,std::string>> testData_t;
            testData_t m_testData;
            string_distance_fixture(){
                m_testData = {
                    {"ulaula", "ohlala"},
                    {"1234567890", "67890"},
                    {"jan", "mann"},
                    {"loster", "lopster"},
                    {"hallo", "hallo"}
                };
            }

            virtual ~string_distance_fixture() {}
            virtual void SetUp() {}
            virtual void TearDown() {}

            void testFunction(int distanceFun(const std::string&, const std::string&)
                             ,std::vector<int> result){
                for (int i = 0; i < result.size(); ++i){
                    int j;
                    EXPECT_EQ(
                        result[i],
                        j = distanceFun(m_testData[i].first, m_testData[i].second)
                    ) << "\n"
                      << "   index: "  << i << std::endl
                      << "   input: '" << m_testData[i].first
                      << "' and '"     << m_testData[i].second << "'\n"
                      << "  result: "  << j << std::endl
                      << "expected: "  << result[i]
                      << "\n\n";
                }
            }
    };

    TEST_F(string_distance_fixture, length){
        testFunction(length, {0, 5, 1 , 1, 0});
    }

    TEST_F(string_distance_fixture, edit_matrix){
        testFunction(edit_matrix, {3, 5, 2 , 1, 0});
    }

    TEST_F(string_distance_fixture, edit_fast){
        testFunction(edit_fast, {3, 5, 2 , 1, 0});
    }
}

