// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_STRUCTURES_MULTI_ARRAY_HEADER
#define OBI_STRUCTURES_MULTI_ARRAY_HEADER

#include <array>
#include <type_traits>
#include <iostream>

namespace obi { namespace structures {


//! Multi Array Class
template <typename _value_type, int... sizes>
class multi_array
{
public:
    using value_type = _value_type;
    int size(){ return this->mysize; }

private:
    static const int mysize = array_size<sizes...>();
    std::array<value_type, mysize> _array;

private:
    //! size function that calculates the length
    //  required length of the array
    template<int last>
    constexpr static
    int
    array_size()
    {
        return last;
    }

    template<int head, int ...tail>
    constexpr static
    typename std::enable_if< sizeof...(tail) != 0, int>::type
    array_size()
    {
        return head*array_size<tail...>();
    }

    at(int last)
    {

    }

    //accessor
    at(int head, int... tail)
    {

    }

};

}}  // obi::structures
#endif // OBI_STRUCTURES_MULTI_ARRAY_HEADER