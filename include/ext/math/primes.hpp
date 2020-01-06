// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// TODO enable_if for integral types
#ifndef EXT_MATH_PRIMES_HEADER
#define EXT_MATH_PRIMES_HEADER

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include <ext/macros/compiler.hpp>

namespace ext { namespace math {

template<typename T>
std::vector<T> prime_factors_naive(T x, bool debug = false) {
    std::vector<T> rv;
    // get rid of the half
    while (x % T(2) == 0) {
        x /= T(2);
        rv.emplace_back(2);
    }
    for (T i(3); i <= x; i += T(2)) {
        if (debug && i % T(1000001) == 0) {
            std::cerr << "at: " << i << std::endl;
        }
        while (x % i == 0) {
            x /= i;
            rv.push_back(i);
        }
    }
    return rv;
}

//! struct that holds a Sieve of Eratosthenes
struct sieve_of_eratosthenes {
    std::vector<bool> sieve; //!< the sieve
    std::size_t max_value;   //!< primes are calculated up to this number
};

//! Calculates the Sieve of Eratosthenes for all primes below a certain number
/*  @param max_value maximum size of prime number
 *  @return pair of nth prime and corresponding index  or  0 on failure
 *
 *  Notes: Just works for size_t and is memory heavy for real stuff I need
 *  a quadratic sieve.
 */
inline sieve_of_eratosthenes create_sieve_of_eratosthenes(std::size_t max_value) {
    std::size_t sieve_size = max_value >> 1;

    sieve_of_eratosthenes result;
    result.sieve = std::vector<bool>(sieve_size, true);
    result.max_value = max_value;

    std::vector<bool>& sieve = result.sieve;

    std::size_t current_pos = 1;
    while (current_pos <= std::size_t(std::sqrt(sieve_size)) + 1) {
        while (sieve[current_pos] == false && current_pos < sieve_size) {
            ++current_pos;
        }

        std::size_t distance = current_pos * 2 + 1;
        for (std::size_t i = current_pos + distance; i < sieve_size; i += distance) {
            if (i >= sieve_size || i < current_pos) {
                break;
            }
            sieve[i] = false;
        }
        ++current_pos;
    }
    return result;
}

#ifdef EXT_COMPILER_GCC
template <std::size_t N>
struct sieve_of_eratosthenes_compile_time {
    std::array<bool,N> sieve; //!< the sieve
    std::size_t max_value;   //!< primes are calculated up to this number
    constexpr sieve_of_eratosthenes_compile_time() : sieve(), max_value(N) {}
};

template <std::size_t max_value>
inline constexpr auto create_sieve_of_eratosthenes() -> sieve_of_eratosthenes_compile_time<(max_value >> 1)> {
    std::size_t constexpr sieve_size = max_value >> 1;

    sieve_of_eratosthenes_compile_time<sieve_size> result;
    for(auto& e : result.sieve) { e = true; }
    result.max_value = max_value;

    auto& sieve = result.sieve;

    std::size_t current_pos = 1;
    while (current_pos <= std::size_t(std::sqrt(sieve_size)) + 1) {
        while (sieve[current_pos] == false && current_pos < sieve_size) {
            ++current_pos;
        }

        std::size_t distance = current_pos * 2 + 1;
        for (std::size_t i = current_pos + distance; i < sieve_size; i += distance) {
            if (i >= sieve_size || i < current_pos) {
                break;
            }
            sieve[i] = false;
        }
        ++current_pos;
    }
    return result;
}
#endif

//! struct that holds primes of maximal size_t size
struct prime_number {
    std::size_t value; //!< numeric value
    std::size_t n;     //!< position the sequence of primes
    std::size_t index; //!< index in the sieve where the number is found
};

//! Calculates the Sieve of Eratosthenes for all primes below a certain number
/*  @param n prime to find
 *  @param sieve returned by sieve_of_eratosthenes
 *  @param use_exceptions flag to signal failure by exception
 *  @return prime_number struct. result.n holds the nth prime or 0 on failure
 */
inline prime_number find_nth_prime(std::size_t n, const sieve_of_eratosthenes& sieve, bool use_exceptions = false) {
    prime_number result;
    result.value = 0;
    result.n = 0;
    result.index = 0;
    if (n == 1) {
        result.value = 2;
        result.n = 1;
        return result;
    }

    // sieve: 1 3 5
    // so we begin with index 1
    std::size_t current = 1; // start with 2
    std::size_t index = 1;   // continue with 3, 5, ...

    while (true) {
        std::size_t number = 2 * index + 1;

        if (sieve.sieve[index]) {
            ++current;
            if (current == n) {
                result.value = number;
                result.index = index;
                result.n = n;
                return result;
            }
        }

        if (number >= sieve.max_value) {
            if (use_exceptions) {
                throw std::logic_error("you do not search in the valid range");
            }
            return result;
        }
        index++;
    }
    return result;
}

//! Calculates the Sieve of Eratosthenes for all primes below a certain number
/*  @param n prime to find
 *  @param primes above this number will not be calculated in the underlying
 * sieve
 *  @param use_exceptions flag to signal failure by exception
 *  @return prime_number struct. result.n holds the nth prime or 0 on failure
 */
inline prime_number find_nth_prime(std::size_t n, std::size_t max_value, bool use_exceptions = false) {
    if (n == 1) {
        prime_number result;
        result.value = 2;
        result.n = 1;
        result.index = 0;
        return result;
    }

    sieve_of_eratosthenes sieve = ext::math::create_sieve_of_eratosthenes(max_value);
    return find_nth_prime(n, sieve, use_exceptions);
}

//! Calculates the next prime
/*  @param current_prime
 *  @param sieve returned by sieve_of_eratosthenes
 *  @param use_exceptions flag to signal failure by exception
 *  @return prime_number struct. result.n holds the nth prime or 0 on failure
 */
inline prime_number&
    find_next_prime(prime_number& current_prime, const sieve_of_eratosthenes& sieve, bool use_exceptions = false) {
    std::size_t index = current_prime.index + 1;
    current_prime.value = 0;

    while (true) {
        std::size_t number = 2 * index + 1;

        if (sieve.sieve[index]) {
            current_prime.n++;
            current_prime.value = number;
            current_prime.index = index;
            return current_prime;
        }

        if (number >= sieve.max_value) {
            if (use_exceptions) {
                throw std::logic_error("you do not search in the valid range");
            }
            return current_prime;
        }
        index++;
    }
    return current_prime;
}
}}     // namespace ext::math
#endif // EXT_MATH_PRIMES_HEADER
