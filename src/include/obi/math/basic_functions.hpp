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

namespace obi { namespace math {

    //calculates product: n * n-1 * ... * k+1 * k
    template<typename T>
    T factors_from_down_to(T n, T k) {
        T rv(1);
        for(T i = k; i <= n; ++i) {
            rv *= i;
        }
        return rv;
    }

    //calculates product: n * n-1 * ... * 2 * 1
    template<typename T>
    T factorial(T n) {
        return factors_from_down_to(n,T(1));
    }

    //calculates: (n * n-1 * ... * k+1 * k) / ( (n-k) * (n-k-1) * ... * 1)
    template<typename T>
    T binomial_coefficient(T n, T k) {
        T nk(n-k);

        if (k <= nk){
            std::swap(k,nk);
        }

        auto numerator = factors_from_down_to(n,k+T(1));
        auto denominator = factorial(nk);

        return numerator / denominator;
    }

    //checkes if decimal number is a palindrome
    template <typename T>
    bool is_palindrome_number(T number, T base = T(10)) {
    // ATTENTION: operations need to support the base
        T copy = number, reversed = 0, digit, rev;
        while (number > 0) {
             reversed = reversed * base + number % base;
             number = number / base;
        }
        return copy == reversed;
    }

}}  // obi::math
