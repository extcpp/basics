// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include <iostream>
#include <iomanip>

namespace obi { namespace util {

template<class T>
class tracing_class {
    T _value;
    static unsigned const _width = 28;

public:
    // default constructor
    tracing_class() : _value() {
        std::cout << std::setw(_width) <<
            "Default Constructor:" << _value << std::endl;
    }

    // ctor
    tracing_class(T v) : _value(v) {
        std::cout << std::setw(_width) <<
            "Copy Constructor, value:" << _value << std::endl;
    }

    // copy constructor
    tracing_class(tracing_class const& other) : _value(other._value) {
        std::cout << std::setw(_width) <<
            "Copy Constructor, value:" << _value << std::endl;
    }

    // move constructor
    tracing_class(tracing_class&& other) : _value(std::move(other._value)) {
        std::cout << std::setw(_width) <<
            "Move Constructor, value:" << _value << std::endl;
    }

    // copy assign
    tracing_class& operator= (tracing_class const& other) {
        if(this != &other){
            _value = other._value;
        }
        std::cout << std::setw(_width) <<
            "Copy assign, value" << _value << std::endl;
    }
    // move assign
    tracing_class& operator= (tracing_class&& other){
        if(this != &other) {
            _value = std::move(other._value);
        }
        std::cout << std::setw(_width) <<
            "Move assign, value:" << _value << std::endl;
    }

    // destructor
    ~tracing_class() {
        std::cout << std::setw(_width) <<
            "Destructor, value:" << _value << std::endl;
    }

    // member
    T set_value(T const v) {
        _value = v;
        std::cout << std::setw(_width) <<
            "Constructor, value:" << _value << std::endl;
    }
};
}}  // namespace obi::util

#ifdef __GNUC__
#define OBI_FUNCTION_NAME { \
    std::cout << __PRETTY_FUNCTION__ << std::endl; \
}
#else
    #define OBI_FUNCTION_NAME
#endif

namespace obi { namespace util {
namespace tracing {

struct all {
    all() {
        OBI_FUNCTION_NAME
    }
    all(all const&) {
        OBI_FUNCTION_NAME
    }
    all(all&&) {
        OBI_FUNCTION_NAME
    }
    all& operator=(all&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    all& operator=(all&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

struct no_default {
    no_default() = delete;
    no_default(no_default const&) {
        OBI_FUNCTION_NAME
    }
    no_default(no_default&&) {
        OBI_FUNCTION_NAME
    }
    no_default& operator=(no_default&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    no_default& operator=(no_default&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

struct no_move {
    no_move() {
        OBI_FUNCTION_NAME
    }
    no_move(no_move const&) {
        OBI_FUNCTION_NAME
    }
    no_move(no_move&&) = default;
    no_move& operator=(no_move&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    no_move& operator=(no_move&&) = default;
};

struct no_move_ctor {
    no_move_ctor() {
        OBI_FUNCTION_NAME
    }
    no_move_ctor(no_move_ctor const&) {
        OBI_FUNCTION_NAME
    }
    no_move_ctor(no_move_ctor&&) = default;
    no_move_ctor& operator=(no_move_ctor&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    no_move_ctor& operator=(no_move_ctor&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

struct no_move_assign {
    no_move_assign() {
        OBI_FUNCTION_NAME
    }
    no_move_assign(no_move_assign const&) {
        OBI_FUNCTION_NAME
    }
    no_move_assign(no_move_assign&&) {
        OBI_FUNCTION_NAME
    }
    no_move_assign& operator=(no_move_assign&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    no_move_assign& operator=(no_move_assign&&) = default;
};

struct no_copy {
    no_copy() {
        OBI_FUNCTION_NAME
    }
    no_copy(no_copy const&) = default;
    no_copy(no_copy&&) {
        OBI_FUNCTION_NAME
    }
    no_copy& operator=(no_copy&) = default;
    no_copy& operator=(no_copy&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

struct no_copy_ctor {
    no_copy_ctor() {
        OBI_FUNCTION_NAME
    }
    no_copy_ctor(no_copy_ctor const&) = default;
    no_copy_ctor(no_copy_ctor&&) {
        OBI_FUNCTION_NAME
    }
    no_copy_ctor& operator=(no_copy_ctor&) {
        OBI_FUNCTION_NAME
        return *this;
    };
    no_copy_ctor& operator=(no_copy_ctor&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

struct no_copy_assign {
    no_copy_assign() {
        OBI_FUNCTION_NAME
    }
    no_copy_assign(no_copy_assign const&) {
        OBI_FUNCTION_NAME
    }
    no_copy_assign(no_copy_assign&&) {
        OBI_FUNCTION_NAME
    }
    no_copy_assign& operator=(no_copy_assign&) = default;
    no_copy_assign& operator=(no_copy_assign&&) {
        OBI_FUNCTION_NAME
        return *this;
    };
};

}  // namespace tracing
}} // namespace obi::util
