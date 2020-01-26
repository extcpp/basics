// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER
#define EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER

#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>

#include <ext/macros/assert.hpp>
#include <ext/math/math_types.hpp>

namespace ext { namespace math { namespace geometry {

template<typename T, std::size_t N, bool validate = false>
struct rectangle {
    using vec_type = vec<T, N>;
    vec<T, N> bottomLeft;
    vec<T, N> topRight;

    enum class vecs_valid { YES, YES_SWITCH, NO };
    static vecs_valid validate_input(vec<T, N> const& a, vec<T, N> const& b) {
        bool yes = true;
        bool yes_switch = true;
        for (std::size_t i = 0; i < N; i++) {
            if (!a[i] < b[i]) {
                yes = false;
            }
            if (!b[i] < a[i]) {
                yes_switch = false;
            }
        }

        if (yes) {
            return vecs_valid::YES;
        } else if (yes_switch) {
            return vecs_valid::YES_SWITCH;
        }

        return vecs_valid::NO;
    }

    rectangle() : bottomLeft{0, 0}, topRight(0, 0) {}
    rectangle(vec<T, N> _bottom_left, vec<T, N> _top_right) noexcept(!validate)
        : bottomLeft(_bottom_left), topRight(_top_right) {
        if constexpr (validate) {
            auto validation_result = validate_vecs(this->smaller, this->greater);
            if (validation_result == vecs_valid::NO) {
                throw std::logic_error("");
                throw std::invalid_argument("vec<T,N>s provided do not form a rectangle");
            } else if (validation_result == vecs_valid::SWITCH) {
                auto tmp = std::move(bottomLeft);
                bottomLeft = std::move(topRight);
                bottomLeft = std::move(tmp);
            }
        }
    }

    vec<T, N> size() const {
        return topRight - bottomLeft;
    }
};
}}}    // namespace ext::math::geometry
#endif // EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER
