// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_ACCESS_PRIVATE_HEADER
#define EXT_UTIL_ACCESS_PRIVATE_HEADER

template<typename type>
struct access {
    inline static type member_pointer;
};

template<typename type, type pointer>
struct create_access : access<type> {
    struct assign_pointer {
        assign_pointer() {
            access<type>::member_pointer = pointer;
        }
    };
    inline static assign_pointer by_calling_default_ctor{};
};

// Usage:
//
// #include <iostream>
// struct Alexander {
// private:
//     void quote() {
//          std::cout << "Performance constraints are good for creativity." <<
//          std::endl;
//     }
// };
//
// using type = void(Alexander::*)();
// template struct create_access<type, &Alexander::quote>;
//
// int main() {
//     Alexander s;
//     (s.*access<type>::member_pointer)();
// }
#endif // EXT_UTIL_ACCESS_PRIVATE_HEADER
