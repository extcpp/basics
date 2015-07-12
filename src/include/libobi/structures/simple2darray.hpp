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

    class SubscriptionProxy{
        typename data::iterator _begin, _end;
    public:
        SubscriptionProxy(typename data::iterator begin) : _begin(begin) {};
        T& operator[](std::size_t const j){
            return *(_begin + j);
        }
    };

    SubscriptionProxy operator[](std::size_t i){
        return SubscriptionProxy(_data.begin() + i * _cols);
    }
};

}}  // obi::structures
