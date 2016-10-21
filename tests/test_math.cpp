#include <gtest/gtest.h>
#include <obi/math/primes.hpp>
#include <obi/math/basic_functions.hpp>
#include <obi/util/basic.hpp>
#include <algorithm>

TEST(math_find_nth_prime, 10001){
    auto sieve = obi::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(obi::math::find_nth_prime(10001, sieve).value
             ,std::size_t(104743)
             );
}

TEST(basic_functions, subsets){
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> expect;
    std::vector<int> in;

    in={};
    result = obi::math::subsets(in);
    expect = {{}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1};
    result = obi::math::subsets(in);
    expect = {{1},{}};
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);

    in={1,2};
    result = obi::math::subsets(in);
    expect = {{},{1},{2},{1,2}};
    //std::sort(result.begin(),result.end());
    //std::sort(expect.begin(),expect.end());
    obi::util::sort_all(result,expect);
    EXPECT_EQ(result, expect);
}
