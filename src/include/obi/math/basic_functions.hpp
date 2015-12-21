// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// TODO enable_if for integral types

#include <utility>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

namespace obi { namespace math {

    template<typename T>
    T factors_from_down_to(T n, T k){
        T rv(1);
        for(T i = k; i <= n; ++i) {
            rv *= i;
        }
        return rv;
    }

    template<typename T>
    T factorial(T n){
        return factors_from_down_to(n,T(1));
    }

    template<typename T>
    T binomial_coefficient(T n, T k){
        T nk(n-k);

        if (k <= nk){
            std::swap(k,nk);
        }

        auto numerator = factors_from_down_to(n,k+T(1));
        auto denominator = factorial(nk);

        return numerator / denominator;
    }

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
            while(x % i == 0){
                x /= i;
                rv.push_back(i);
            }
        }

        return rv;
    }
}}  // obi::math
