// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER
#define EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER

#include <array>
#include <stdexcept>

namespace ext { namespace math { namespace geometry {


    template <typename T, std::size_t N>
    struct point {
        std::array<T,N> data;

        point(point const&) = default;

        template <typename ...Forward
                 ,typename = std::enable_if_t<!(std::is_same_v<point<T,N>,std::decay_t<Forward>> && ...)>
                 >
        point(Forward&& ...f): data{std::forward<Forward>(f)...} {}

        T& get_x() {
            static_assert(N > 0);
            return this->data[0];
        }

        T& get_y() {
            static_assert(N > 1);
            return this->data[1];
        }

        T& get_z() {
            static_assert(N > 2);
            return this->data[2];
        }

        T& get_x() const {
            static_assert(N > 0);
            return this->data[0];
        }

        T& get_y() const {
            static_assert(N > 1);
            return this->data[1];
        }

        T& get_z() const {
            static_assert(N > 2);
            return this->data[2];
        }

        T& operator[](std::size_t index){
            return data[index];
        }

        T const& operator[](std::size_t index) const {
            return data[index];
        }

        std::size_t size() const {
            return data.size();
        }
    };

    template <typename T, std::size_t N>
    point<T, N> operator-(point<T,N> const& left, point<T,N> const& right) {
        auto rv = left;
        for(std::size_t i = 0; i < N; ++i){
            rv[i]-=right[i];
        }
        return rv;
    }

    template <typename T, std::size_t N, bool validate = false>
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
            : smaller(a), greater(b) {
            if constexpr (validate) {
                auto validation_result = validate_points(this->smaller, this->greater);
                if (validation_result == points_valid::NO){
                    throw std::logic_error("");
                    throw std::invalid_argument("point<T,N>s provided do not form a rectangle");
                } else if (validation_result == points_valid::SWITCH) {
                    auto tmp = std::move(smaller);
                    smaller = std::move(greater);
                    greater = std::move(tmp);
                }
            }
        }

        point<T,N> size(){
            return greater - smaller;
        }
    };

}}}
#endif // EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER
