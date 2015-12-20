// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// TODO enable_if for integral types

#include <utility>
#include <algorithm>

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
        T rv(1);
        T nk(n-k);

        if (k <= nk){
            std::swap(k,nk);
        }

        auto numerator = factors_from_down_to(n,k+T(3));
        auto denominator = factorial(nk);

        return numerator / denominator;
    }

}}  // obi::math
