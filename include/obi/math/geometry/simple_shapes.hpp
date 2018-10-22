#pragma once
#ifndef _OBI_MATH_GEOMETRY_SIMPLE_SHAPES
#define _OBI_MATH_GEOMETRY_SIMPLE_SHAPES

#include <array>
#include <stdexcept>

namespace obi { namespace math { namespace geometry {

    template <typename T, int N>
    struct point {
        std::array<T,N> data;

        template<typename return_type = T>
        std::enable_if_t< (N>0), return_type >
        get_x() const {
            return this->data[0];
        }

        template<typename return_type = T>
        std::enable_if_t< (N>1), return_type >
        get_y() const {
            return this->data[1];
        }

        template<typename return_type = T>
        std::enable_if_t< (N>2), return_type >
        get_z() const {
            return this->data[2];
        }
    };

    template <typename T, int N, bool validate = false>
    struct rectangle {
        point<T,N> smaller;
        point<T,N> greater;

        enum class points_valid { YES, YES_SWITCH, NO };
        static points_valid validate_input (point<T,N> const& a, point<T,N> const& b){
            bool yes = true;
            bool yes_switch = true;
            for (std::size_t i = 0; i < N; i++) {
                if (! a[i] < b[i]) { yes = false; }
                if (! b[i] < a[i]) { yes_switch = false; }
            }

            if (yes) { return points_valid::YES; }
            else if (yes_switch) { return points_valid::YES_SWITCH; }

            return points_valid::NO;
        }

        rectangle(point<T,N> a, point<T,N> b) noexcept(!validate)
            : smaller(std::move(a)), greater(std::move(b)) {
            if constexpr (validate) {
                auto validation_result = validate_points(this->smaller, this->greater);
                if (validation_result == points_valid::NO){
                    throw std::invalid_argument("point<T,N>s provided do not form a rectangle");
                } else if (validation_result == points_valid::SWITCH) {
                    auto tmp = std::move(smaller);
                    smaller = std::move(greater);
                    greater = std::move(tmp);
                }
            }
        }
    };

}}}
#endif
