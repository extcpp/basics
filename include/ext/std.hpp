// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_STD_HEADER
#define EXT_STD_HEADER
#include <vector>

namespace std {
// add get for vector - be sure what you do when using it
template<typename std::size_t _Int, typename _Type>
constexpr _Type& get(vector<_Type>& _vec) {
    return _vec[_Int];
}
} // namespace std

#endif // EXT_STD_HEADER
