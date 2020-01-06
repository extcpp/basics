#include <algorithm>
#include <gtest/gtest.h>

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

//#ifdef EXT_TEST_LONG
TEST(math_find_nth_prime, 10001) {
    auto sieve = ext::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(ext::math::find_nth_prime(10001, sieve).value, std::size_t(104743));
}
//#endif

#ifdef EXT_COMPILER_GCC
TEST(math_sieve, compile_time) {
    constexpr auto sieve_compile = ext::math::create_sieve_of_eratosthenes<4000>();
    auto sieve_runtime = ext::math::create_sieve_of_eratosthenes(4000);
    bool res = std::equal(sieve_compile.sieve.begin(), sieve_compile.sieve.end(), sieve_runtime.sieve.begin());
    EXPECT_TRUE(res);
}

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
#endif

TEST(types, static_matrix_i_access) {

    ext::math::static_matrix<std::size_t, 3, 2, true /*opt i access*/, true /*checked*/>
        // clang-format off
        m { 1, 2
          , 3, 4
          , 5, 6};
    // clang-format on

    std::array<std::size_t, 6> data = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(m.data, data);

    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(0, 1), 2);
    EXPECT_EQ(m.get(1, 0), 3);
    EXPECT_EQ(m.get(1, 1), 4);
    EXPECT_EQ(m.get(2, 0), 5);
    EXPECT_EQ(m.get(2, 1), 6);

    EXPECT_THROW(m.get(2, 3), std::runtime_error);
}

TEST(types, static_matrix_j_access) {

    // still - but different data layout
    // 1 2
    // 3 4
    // 5 6

    ext::math::static_matrix<std::size_t, 3, 2, false /*opt i access*/, true /*checked*/>
        // clang-format off
        m { 1, 3 , 5
          , 2, 4, 6 };
    // clang-format on

    std::array<std::size_t, 6> data = {1, 3, 5, 2, 4, 6};
    EXPECT_EQ(m.data, data);

    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(0, 1), 2);
    EXPECT_EQ(m.get(1, 0), 3);
    EXPECT_EQ(m.get(1, 1), 4);
    EXPECT_EQ(m.get(2, 0), 5);
    EXPECT_EQ(m.get(2, 1), 6);

    EXPECT_THROW(m.get(2, 3), std::runtime_error);
}

TEST(types, dynamic_matrix_i_access) {

    ext::math::dynamic_matrix<std::size_t, true /*opt i access*/, true /*checked*/> m(3, 2);
    // clang-format off
        m.data = { 1, 2
                 , 3, 4
                 , 5, 6};
    // clang-format on

    {
        std::vector<std::size_t> data = {1, 2, 3, 4, 5, 6};
        EXPECT_EQ(m.data, data);

        EXPECT_EQ(m.get(0, 0), 1);
        EXPECT_EQ(m.get(0, 1), 2);
        EXPECT_EQ(m.get(1, 0), 3);
        EXPECT_EQ(m.get(1, 1), 4);
        EXPECT_EQ(m.get(2, 0), 5);
        EXPECT_EQ(m.get(2, 1), 6);

        EXPECT_THROW(m.get(2, 3), std::runtime_error);
    }

    {
        // test for set and mutable access
        std::vector<std::size_t> data = {1, 2, 3, 4, 5, 8};
        EXPECT_EQ(m.set(2, 1, 8), 8);
        EXPECT_EQ(m.data, data);

        std::vector<std::size_t> data_10 = {10, 2, 3, 4, 5, 8};
        m.get_mutable(0, 0) = 10;
        EXPECT_EQ(m.data, data_10);

        m.get_mutable(0, 0) = 1;
        EXPECT_EQ(m.data, data);
    }
}
