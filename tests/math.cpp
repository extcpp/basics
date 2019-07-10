#include <gtest/gtest.h>
#include <algorithm>

#include <ext/math/basic_functions.hpp>
#include <ext/math/math_types.hpp>
#include <ext/math/primes.hpp>
#include <ext/util/basic.hpp>

TEST(binomial_coefficient, std) {
    EXPECT_EQ(ext::math::binomial_coefficient(5, 3), 10);
}

TEST(binomial_coefficient, better) {
    EXPECT_EQ(ext::math::binomial_coefficient_dynamic(5, 3), 10);
}

#ifdef EXT_TEST_LONG
TEST(math_find_nth_prime, 10001) {
    auto sieve = ext::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(ext::math::find_nth_prime(10001, sieve).value, std::size_t(104743));
}
#endif

TEST(basic_functions, powersets) {
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> expect;
    std::vector<int> in;

    in = {};
    result = ext::math::powersets(in);
    expect = {{}};
    ext::util::sort_all(result, expect);
    EXPECT_EQ(result, expect);

    in = {1};
    result = ext::math::powersets(in);
    expect = {{1}, {}};
    ext::util::sort_all(result, expect);
    EXPECT_EQ(result, expect);

    in = {1, 2};
    result = ext::math::powersets(in);
    expect = {{}, {1}, {2}, {1, 2}};
    ext::util::sort_all(result, expect);
    EXPECT_EQ(result, expect);

    in = {1, 2, 3};
    result = ext::math::powersets(in);
    expect = {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}};
    ext::util::sort_all(result, expect);
    EXPECT_EQ(result, expect);
}

TEST(types, matrix_i_access) {

    ext::math::matrix<std::size_t,3,2, true /*opt i access*/, true /*checked*/>
        m { 1, 2
          , 3, 4
          , 5, 6};

    std::array<std::size_t,6> data = {1,2,3,4,5,6};
    EXPECT_EQ(m.data, data);

    EXPECT_EQ(m.get(0,0),1);
    EXPECT_EQ(m.get(1,0),2);
    EXPECT_EQ(m.get(0,1),3);
    EXPECT_EQ(m.get(1,1),4);
    EXPECT_EQ(m.get(0,2),5);
    EXPECT_EQ(m.get(1,2),6);

    EXPECT_THROW(m.get(2,3), std::runtime_error);

}

TEST(types, matrix_j_access) {

    // still - but different data layout
    // 1 2
    // 3 4
    // 5 6

    ext::math::matrix<std::size_t,3,2, false /*opt i access*/, true /*checked*/>
        m { 1, 3 , 5
          , 2, 4, 6 };

    std::array<std::size_t,6> data = {1,3,5,2,4,6};
    EXPECT_EQ(m.data, data);

    EXPECT_EQ(m.get(0,0),1);
    EXPECT_EQ(m.get(1,0),2);
    EXPECT_EQ(m.get(0,1),3);
    EXPECT_EQ(m.get(1,1),4);
    EXPECT_EQ(m.get(0,2),5);
    EXPECT_EQ(m.get(1,2),6);

    EXPECT_THROW(m.get(2,3), std::runtime_error);

}
