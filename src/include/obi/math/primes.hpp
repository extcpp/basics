// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// TODO enable_if for integral types
#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>

namespace obi { namespace math {

    template<typename T>
    std::vector<T> prime_factors_naive(T x, bool debug = false) {
        std::vector<T> rv;
        //get rid of the half
        while(x % T(2) == 0) {
            x /= T(2);
            rv.emplace_back(2);
        }
        for (T i(3); i <= x; i += T(2)) {
            if (debug && i % T(1000001) == 0) {
                std::cerr << "at: " << i << std::endl;
            }
            while(x % i == 0) {
                x /= i;
                rv.push_back(i);
            }
        }
        return rv;
    }

    struct ESieve {
        std::vector<bool> sieve;
        std::size_t max_number;
    };


    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param max_number maximum size of prime number
     *  @return pair of nth prime and corresponding index  or  0 on failure
     *
     *  Notes: Just works for size_t and is memory heavy for real stuff I need Quadratic Sieve
    */
    ESieve sieve_of_eratosthenes(std::size_t max_number) {
        std::size_t sieve_size = max_number >> 1;

        ESieve result;
        result.sieve = std::vector<bool>(sieve_size, true);
        result.max_number = max_number;

        auto& sieve = result.sieve;

        std::size_t current_pos = 1;
        while(current_pos <= std::sqrt(sieve_size)+1) {
            while(sieve[current_pos] == false && current_pos < sieve_size) {
                ++current_pos;
            }

            std::size_t distance = (current_pos << 1) + 1;
            for(std::size_t i = current_pos + distance; i < sieve_size; i += distance) {
                if( i >= sieve_size || i < current_pos ){
                    break;
                }
                sieve[i] = false;
            }
            ++current_pos;
        }
        return result;
    }


    struct Prime {
        std::size_t number;
        std::size_t n;
        std::size_t index;
    };

    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param n prime to find
     *  @param sieve retruned by sieve_of_eratosthenes
     *  @param use_exceptions flag to signal failure by exception
     *  @return Prime struct. result.n holds the nth prime or 0 on failure
    */
    Prime find_nth_prime(std::size_t n, ESieve sieve, bool use_exceptions = false) {

        Prime result;
        result.number=0;
        result.n=0;
        result.index=0;
        if(n==1) {
            result.number = 2;
            result.n = 1;
            return result;
        }


        //sieve: 1 3 5
        //so we begin with index 1
        std::size_t current = 1; // start with 2
        std::size_t index = 1;   // continue with 3, 5, ...

        while(true) {
            std::size_t number = 2*index + 1;

            if (sieve.sieve[index]) {
                ++current;
                if (current == n) {
                    result.number = number;
                    result.index = index;
                    result.n = n;
                    return result;
                }
            }

            if(number >= sieve.max_number){
                if(use_exceptions){
                    throw std::logic_error("you do not search in the vaild range");
                }
                return result;
            }
            index++;
        }
        return result;
    }

    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param n prime to find
     *  @param primes above this number will not be calculated in the underlying sieve
     *  @param use_exceptions flag to signal failure by exception
     *  @return Prime struct. result.n holds the nth prime or 0 on failure
    */
    Prime find_nth_prime(std::size_t n, std::size_t max_prime, bool use_exceptions = false) {

        if(n==1) {
            Prime result;
            result.number = 2;
            result.n = 1;
            result.index=0;
            return result;
        }

        auto sieve = obi::math::sieve_of_eratosthenes(max_prime);
        return find_nth_prime(n, sieve, use_exceptions);
    }

    //! Calculates the next prime
    /*  @param current_prime
     *  @param sieve returned by sieve_of_eratosthenes
     *  @param use_exceptions flag to signal failure by exception
     *  @return Prime struct. result.n holds the nth prime or 0 on failure
    */
    Prime& find_next_prime(Prime& current_prime, const ESieve& sieve, bool use_exceptions = false) {
        std::size_t index = current_prime.index + 1;
        current_prime.number = 0;

        while(true) {
            std::size_t number = 2*index + 1;

            if (sieve.sieve[index]) {
                current_prime.n++;
                current_prime.number = number;
                current_prime.index = index;
                return current_prime;
            }

            if(number >= sieve.max_number){
                if(use_exceptions){
                    throw std::logic_error("you do not search in the vaild range");
                }
                return current_prime;
            }
            index++;
        }
        return current_prime;
    }

}}  // obi::math
