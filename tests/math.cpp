#include <gtest/gtest.h>
#include <ext/math/primes.hpp>
#include <ext/math/basic_functions.hpp>
#include <ext/util/basic.hpp>
#include <algorithm>

TEST(binomial_coefficient, std){
    EXPECT_EQ(ext::math::binomial_coefficient(5,3),10);
}

TEST(binomial_coefficient, better){
    EXPECT_EQ(ext::math::binomial_coefficient_dynamic(5,3),10);
}

#ifdef EXT_TEST_LONG
TEST(math_find_nth_prime, 10001){
    auto sieve = ext::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(ext::math::find_nth_prime(10001, sieve).value
             ,std::size_t(104743)
             );
}
#endif

TEST(basic_functions, powersets){
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> expect;
    std::vector<int> in;

    in={};
    result = ext::math::powersets(in);
    expect = {{}};
    ext::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1};
    result = ext::math::powersets(in);
    expect = {{1},{}};
    ext::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1,2};
    result = ext::math::powersets(in);
    expect = {{},{1},{2},{1,2}};
    ext::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1,2,3};
    result = ext::math::powersets(in);
    expect = {{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}};
    ext::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);
}
