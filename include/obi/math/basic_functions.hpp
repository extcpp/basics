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

    //create vector of sets form fiven vector
    template<typename T>
    std::vector<std::vector<T>> subsets(std::vector<T> const& in){
        std::vector<std::vector<int>> rv{};
        rv.push_back(in);
        rv.push_back({});
        std::size_t in_size = in.size();
        std::vector<char> bit(in.size(),1);
        bool even = (in_size % 2 == 0) ? true : false;
        std::size_t split_point = in_size/2;
        for(std::size_t i = 0; i < split_point; ++i){
            bit[i]=0;
            do {
                std::vector<int> tmp1;
                tmp1.reserve(i+1);
                std::vector<int> tmp2;
                tmp2.reserve(in_size-(i+1));
                for(std::size_t j = 0; j < in.size(); ++j){
                    if(bit[j]){
                        tmp1.push_back(in[j]);
                    } else {
                        tmp2.push_back(in[j]);
                    }
                }
                if((i != split_point-1) || !even){
                    rv.push_back(std::move(tmp1));
                    rv.push_back(std::move(tmp2));
                }
                else if(even){
                    rv.push_back(std::move(tmp1));
                }
            } while (std::next_permutation(bit.begin(),bit.end()));
        }
        return rv;
    }


}}  // obi::math
