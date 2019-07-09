#pragma once
#    include <array>
#    include <vector>
#    include <algorithm>
#    include <ext/macros/assert.hpp>

namespace ext { namespace math {

template<typename T, std::size_t N>
struct vec {
    using value_type = T;
    static constexpr std::size_t size = N;

    std::array<T, N> data;

    vec(vec const&) = default;

    template<typename... Forward,
             typename = std::enable_if_t<!(std::is_same_v<vec<T, N>, std::decay_t<Forward>> && ...)>>
    vec(Forward&&... f) : data{std::forward<Forward>(f)...} {}

    vec(std::vector<T> const& vec)  {
        EXT_ASSERT(N == vec.size());
        std::copy(vec.begin(), vec.end(), data.begin());
    }

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

    T const& get_x() const {
        static_assert(N > 0);
        return this->data[0];
    }

    T const & get_y() const {
        static_assert(N > 1);
        return this->data[1];
    }

    T const & get_z() const {
        static_assert(N > 2);
        return this->data[2];
    }

    T& operator[](std::size_t index) {
        return data[index];
    }

    T const& operator[](std::size_t index) const {
        return data[index];
    }

    std::vector<value_type> to_vector() const {
        std::vector<value_type> vec(size);
        std::copy(data.begin(),data.end(), vec.begin());
        return vec;
    }

};

template<typename T, std::size_t N>
vec<T, N> operator-(vec<T, N> const& left, vec<T, N> const& right) {
    auto rv = left;
    for (std::size_t i = 0; i < N; ++i) {
        rv[i] -= right[i];
    }
    return rv;
}

template<typename T, std::size_t N>
vec<T, N> operator+(vec<T, N> const& left, vec<T, N> const& right) {
    auto rv = left;
    for (std::size_t i = 0; i < N; ++i) {
        rv[i] += right[i];
    }
    return rv;
}


//M rows - N cloumns
template <typename T, std::size_t M, std::size_t N, bool x_linear = true, bool checked = false>
struct matrix {
    using value_type = T;
    constexpr static std::size_t row_count = M;
    constexpr static std::size_t column_count = N;

    std::array<T,row_count * column_count> data;

    void init_all_to(value_type const& value){
        std::fill(data.begin, data.end(), value);
    }

    T const& get(std::size_t x, std::size_t y) const {
        if constexpr(checked) {
            if(x * y > row_count * column_count) {
                throw std::runtime_error("out of range");
            }
        }

        if constexpr(x_linear) {
            return data[x + row_count * y];
        } else {
            return data[y + row_count * x];
        }
    }

    T const& set(std::size_t x, std::size_t y, T const& value) {
        if constexpr(checked) {
            if(x * y > row_count * column_count) {
                throw std::runtime_error("out of range");
            }
        }

        if constexpr(x_linear) {
            return data[x + row_count * y] = value;
        } else {
            return data[y + row_count * x] = value;
        }
    }

    T const& set(std::size_t x, std::size_t y, T&& value) {
        if constexpr(checked) {
            if(x * y > row_count * column_count) {
                throw std::runtime_error("out of range");
            }
        }

        if constexpr(x_linear) {
            return data[x + row_count * y] = std::move(value);
        } else {
            return data[y + row_count * x] = std::move(value);
        }
    }

};

}}
