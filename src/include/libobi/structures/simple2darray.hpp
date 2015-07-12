#pragma once

#include <vector>
#include <type_traits>

namespace obi { namespace structures {


template <typename T>
class array2d
{
    using data = std::vector<T>;
    std::size_t _rows, _cols;
    data _data;

public:
    array2d(std::size_t rows, std::size_t cols): _rows(rows), _cols(cols){
        _data.resize(rows * cols);
    }

    template<typename Iterator>
    class SubscriptionProxy{
        typename data::iterator _begin;
    public:
        SubscriptionProxy(Iterator begin) : _begin(begin) {};
        T& operator[](std::size_t const j){
            return *(_begin + j);
        }
        T const & operator[](std::size_t const j) const {
            return *(_begin + j);
        }
    };

    auto operator[](std::size_t i){
        return SubscriptionProxy<typename data::iterator>(_data.begin() + i * _cols);
    }
    auto operator[](std::size_t i) const {
        return SubscriptionProxy<typename data::const_iterator>(_data.begin() + i * _cols);
    }
};

}}  // obi::structures
