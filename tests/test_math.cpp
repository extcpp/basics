#include <gtest/gtest.h>
#include <obi/math/primes.hpp>

TEST(math_primes_find_nth, find_nth_prime){
    auto sieve = obi::math::create_sieve_of_eratosthenes(40000000);
    EXPECT_EQ(obi::math::find_nth_prime(10001, sieve).value
             ,std::size_t(104743)
             );
}
