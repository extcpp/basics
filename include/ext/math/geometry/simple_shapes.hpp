// Copyright - 2017-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
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

template<typename T, std::size_t N, bool Validate = false>
struct rectangle {
    using vec_type = vec<T, N>;
    vec<T, N> bottomLeft;
    vec<T, N> topRight;

    bool validate() const {
        bool valid = true;
        for (std::size_t i = 0; i < N; i++) {
            if (bottomLeft[i] >= topRight[i]) {
                valid = false;
            }
        }
        return valid;
    }

    rectangle() : bottomLeft{0, 0}, topRight(0, 0) {}
    rectangle(vec<T, N> _bottom_left, vec<T, N> _top_right) noexcept(!Validate)
        : bottomLeft(_bottom_left), topRight(_top_right) {
        if constexpr (Validate) {
            auto validation_result = validate();
            if (!validation_result) {
                throw std::invalid_argument("vec<T,N>s provided do not form a rectangle");
            }
        }
    }

    vec<T, N> size() const {
        return topRight - bottomLeft;
    }
};
}}}    // namespace ext::math::geometry
#endif // EXT_MATH_GEOMETRY_SIMPLE_SHAPES_HEADER
