// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include <iostream>
#include <iomanip>
#include <obi/meta/is_one_of.hpp>

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

struct base {
    bool default_ctor = false;
    bool copy_ctor = false;
    bool copy_op = false;
    bool move_ctor = false;
    bool move_op = false;

    void print() const {
        std::cout << std::boolalpha
                  << "default ctor: " << default_ctor << std::endl
                  << "copy ctor:    " << copy_ctor << std::endl
                  << "copy assign:  " << copy_op << std::endl
                  << "move ctor:    " << move_ctor << std::endl
                  << "move assign:  " << move_op << std::endl
                  ;
    }

    void reset() {
        default_ctor = false;
        copy_ctor = false;
        copy_op = false;
        move_ctor = false;
        move_op = false;
    }

    void copy_values(base const* other) {
        default_ctor = other->default_ctor;
        copy_ctor = other->copy_ctor;
        copy_op = other->copy_op;
        move_ctor = other->move_ctor;
        move_op = other->move_op;
    }

};

struct all : base {
    all() : base() {
        OBI_FUNCTION_NAME
        default_ctor = true;
    }

    all(all const& other) {
        OBI_FUNCTION_NAME
        copy_values(&other);
        copy_ctor = true;
    }

    all& operator=(all& other) {
        OBI_FUNCTION_NAME
        copy_values(&other);
        copy_op = true;
        return *this;
    }
    all(all&& other) {
        OBI_FUNCTION_NAME
        copy_values(&other);
        move_ctor = true;
    }

    all& operator=(all&& other) {
        OBI_FUNCTION_NAME
        copy_values(&other);
        move_op = true;
        return *this;
    }
};

struct no_default : base {
    no_default() = delete;
};

struct no_move : base {
    no_move(no_move&&) = delete;
    no_move& operator=(no_move&&) = delete;
};

struct no_move_ctor : base {
    no_move_ctor(no_move_ctor&&) = delete;
};

struct no_move_assign : base {
    no_move_assign& operator=(no_move_assign&&) = delete;
};

struct no_copy : base {
    no_copy(no_copy const&) = delete;
    no_copy& operator=(no_copy&) = delete;
};

struct no_copy_ctor : base {
    no_copy_ctor(no_copy_ctor const&) = delete;
};

struct no_copy_assign : base {
    no_copy_assign& operator=(no_copy_assign&) = delete;
};


}  // namespace tracing
}} // namespace obi::util
