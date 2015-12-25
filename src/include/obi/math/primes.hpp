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

    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param max_number maximum size of prime number
     *  @return pair of sieve and and max_number
     *
     *  Notes: Just works for size_t and is memory heavy for real stuff I need Quadratic Sieve
    */
    std::pair<std::vector<bool>,std::size_t> sieve_of_eratosthenes(std::size_t max_number) {
        std::size_t sieve_size = max_number >> 1;
        auto result = std::make_pair(std::vector<bool>( sieve_size, true),max_number); //we just store odd elements
        auto& sieve = result.first;

        std::size_t current_pos = 1;
        while(current_pos <= std::sqrt(sieve_size)+1) {
            while(sieve[current_pos] == false && current_pos < sieve_size) {
                ++current_pos;
            }

            std::size_t distance = (current_pos << 1) + 1;
            for(std::size_t i = current_pos + distance; i < sieve_size; i += distance) {
                if( i >= sieve_size || i < current_pos ){
                    std::cout << "overflow" << std::endl;
                    break;
                }
                sieve[i] = false;
            }
            ++current_pos;
        }
        return result;
    }

    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param n prime to find
     *  @param sieve_pair a pair retruned by sieve_of_eratosthenes
     *  @param use_exceptions flag to signal failure by exception
     *  @return nth prime or 0 on failure
    */
    std::size_t find_nth_prime( std::size_t n
                              , const std::pair<std::vector<bool>, std::size_t>& sieve_pair
                              , bool use_exceptions = false
                              ) {
        if(n==1) {
            return 2;
        }

        auto& sieve = sieve_pair.first;
        auto& max_prime = sieve_pair.second;

        //sieve: 1 3 5
        //so we begin with index 1
        std::size_t current = 1; // start with 2
        std::size_t index = 1;   // continue with 3, 5, ...

        while(true) {
            std::size_t number = 2*index + 1;

            if (sieve[index]) {
                ++current;
                if (current == n) {
                    return number;
                }
            }

            if(number >= max_prime){
                if(use_exceptions){
                    throw std::logic_error("you do not search in the vaild range");
                }
                return 0;
            }
            index++;
        }
        return 0;
    }

    //! Calculates the sieve of eratosthenes for all primes below a certain number
    /*  @param n prime to find
     *  @param primes above this number will not be calculated in the underlying sieve
     *  @param use_exceptions flag to signal failure by exception
     *  @return nth prime or 0 on failure
    */
    std::size_t find_nth_prime(std::size_t n, std::size_t max_prime, bool use_exceptions = false) {
        if(n==1) {
            return 2;
        }
        auto sieve_pair = obi::math::sieve_of_eratosthenes(max_prime);
        return find_nth_prime(n, sieve_pair, use_exceptions);
    }

}}  // obi::math
