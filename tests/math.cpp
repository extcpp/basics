#include <gtest/gtest.h>
#include <obi/math/primes.hpp>
#include <obi/math/basic_functions.hpp>
#include <obi/util/basic.hpp>
#include <algorithm>

TEST(binomial_coefficient, std){
    EXPECT_EQ(obi::math::binomial_coefficient(5,3),10);
}

TEST(binomial_coefficient, better){
    EXPECT_EQ(obi::math::binomial_coefficient_dynamic(5,3),10);
}

TEST(math_find_nth_prime, 10001){
    auto sieve = obi::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(obi::math::find_nth_prime(10001, sieve).value
             ,std::size_t(104743)
             );
}

TEST(basic_functions, powersets){
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> expect;
    std::vector<int> in;

    in={};
    result = obi::math::powersets(in);
    expect = {{}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1};
    result = obi::math::powersets(in);
    expect = {{1},{}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1,2};
    result = obi::math::powersets(in);
    expect = {{},{1},{2},{1,2}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1,2,3};
    result = obi::math::powersets(in);
    expect = {{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);
}
