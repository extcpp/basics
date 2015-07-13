#pragma once

#include <vector>
#include <type_traits>
#include <utility>

namespace obi { namespace structures {

template <typename T>
class array2d
{
    using data = typename std::vector<T>;
    using size_type = typename data::size_type;
    using diff_type = typename data::difference_type;

    size_type _rows, _cols;
    data _data;

public:
    array2d(size_type rows, size_type cols): _rows(rows), _cols(cols){
        _data.resize(rows * cols);
    }

    template<typename Iterator>
    class SubscriptionProxy{
        Iterator _begin;
    public:
        SubscriptionProxy(Iterator begin) : _begin(begin) {};
        typename Iterator::reference operator[](std::size_t const j) const {
            return *(_begin + static_cast<diff_type>(j));
        }
    };

    auto operator[](size_type const i){
        return SubscriptionProxy<typename data::iterator>(_data.begin() + static_cast<diff_type>(i * _cols));
    }
    auto operator[](size_type const i) const {
        return SubscriptionProxy<typename data::const_iterator>(_data.begin() + static_cast<diff_type>(i * _cols));
    }

    auto size() const {
        return std::make_pair(_rows,_cols);
    }
};

template<typename T>
std::ostream& operator <<(std::ostream& stream, obi::structures::array2d<T> const & arr){
    using namespace std;
    auto size = arr.size();
    for(std::size_t j = 0; j < size.first; ++j){
        for(std::size_t i = 0; i < size.second; ++i){
            stream << arr[j][i] << " ";
        }
        stream << endl;
    }
    stream << endl;
    return stream;
}

}}  // obi::structures
