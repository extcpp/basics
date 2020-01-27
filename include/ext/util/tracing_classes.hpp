// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_TRACING_CLASSES_HEADER
#define EXT_UTIL_TRACING_CLASSES_HEADER
#include <ext/meta/is_one_of.hpp>

#include <iomanip>
#include <iostream>

namespace ext { namespace util {

template<class T>
class tracing_class {
    T _value;
    static unsigned const _width = 28;

    public:
    // default constructor
    tracing_class() : _value() {
        std::cout << std::setw(_width) << "Default Constructor:" << _value << std::endl;
    }

    // ctor
    tracing_class(T v) : _value(v) {
        std::cout << std::setw(_width) << "Copy Constructor, value:" << _value << std::endl;
    }

    // copy constructor
    tracing_class(tracing_class const& other) : _value(other._value) {
        std::cout << std::setw(_width) << "Copy Constructor, value:" << _value << std::endl;
    }

    // move constructor
    tracing_class(tracing_class&& other) : _value(std::move(other._value)) {
        std::cout << std::setw(_width) << "Move Constructor, value:" << _value << std::endl;
    }

    // copy assign
    tracing_class& operator=(tracing_class const& other) {
        if (this != &other) {
            _value = other._value;
        }
        std::cout << std::setw(_width) << "Copy assign, value" << _value << std::endl;
    }
    // move assign
    tracing_class& operator=(tracing_class&& other) {
        if (this != &other) {
            _value = std::move(other._value);
        }
        std::cout << std::setw(_width) << "Move assign, value:" << _value << std::endl;
    }

    // destructor
    ~tracing_class() {
        std::cout << std::setw(_width) << "Destructor, value:" << _value << std::endl;
    }

    // member
    T set_value(T const v) {
        _value = v;
        std::cout << std::setw(_width) << "Constructor, value:" << _value << std::endl;
    }
};
}} // namespace ext::util

#if defined(__GNUC__) && defined(EXT_DEBUG)
    #define EXT_FUNCTION_NAME \
        { std::cout << __PRETTY_FUNCTION__ << std::endl; }
#else
    #define EXT_FUNCTION_NAME
#endif // defined(__GNUC__) && defined(EXT_DEBUG)

namespace ext { namespace util { namespace tracing {

struct base {
    std::size_t default_ctor = 0;
    std::size_t non_default_ctor = 0;
    std::size_t copy_ctor = 0;
    std::size_t copy_assign = 0;
    std::size_t move_ctor = 0;
    std::size_t move_assign = 0;

    void print() const {
        std::cout << std::boolalpha << "default ctor:     " << default_ctor << std::endl
                  << "non default ctor: " << non_default_ctor << std::endl
                  << "copy ctor:        " << copy_ctor << std::endl
                  << "copy assign:      " << copy_assign << std::endl
                  << "move ctor:        " << move_ctor << std::endl
                  << "move assign:      " << move_assign << std::endl;
    }

    void reset() {
        default_ctor = 0;
        non_default_ctor = 0;
        copy_ctor = 0;
        copy_assign = 0;
        move_ctor = 0;
        move_assign = 0;
    }

    void copy_values(base const* other) {
        default_ctor = other->default_ctor;
        non_default_ctor = other->non_default_ctor;
        copy_ctor = other->copy_ctor;
        copy_assign = other->copy_assign;
        move_ctor = other->move_ctor;
        move_assign = other->move_assign;
    }

    std::size_t opearations() {
        return default_ctor + non_default_ctor + copy_ctor + copy_assign + move_ctor + move_assign;
    }
};

struct all : base {
    all() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    all(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    all(all const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    all& operator=(all& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }

    all(all&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }

    all& operator=(all&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

struct no_default : base {
    no_default() = delete;

    no_default(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_default(no_default const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    no_default& operator=(no_default& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }

    no_default(no_default&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }

    no_default& operator=(no_default&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

struct no_move : base {
    no_move(no_move&&) = delete;
    no_move& operator=(no_move&&) = delete;

    no_move() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_move(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_move(no_move const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    no_move& operator=(no_move& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }
};

struct no_move_ctor : base {
    no_move_ctor(no_move_ctor&&) = delete;

    no_move_ctor() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_move_ctor(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_move_ctor(no_move_ctor const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    no_move_ctor& operator=(no_move_ctor& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }

    no_move_ctor& operator=(no_move_ctor&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

struct no_move_assign : base {
    no_move_assign& operator=(no_move_assign&&) = delete;

    no_move_assign() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_move_assign(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_move_assign(no_move_assign const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    no_move_assign& operator=(no_move_assign& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }

    no_move_assign(no_move_assign&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }
};

struct no_copy : base {
    no_copy(no_copy const&) = delete;
    no_copy& operator=(no_copy&) = delete;

    no_copy() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_copy(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_copy(no_copy&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }

    no_copy& operator=(no_copy&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

struct no_copy_ctor : base {
    no_copy_ctor(no_copy_ctor const&) = delete;

    no_copy_ctor() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_copy_ctor(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_copy_ctor& operator=(no_copy_ctor& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_assign;
        return *this;
    }

    no_copy_ctor(no_copy_ctor&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }

    no_copy_ctor& operator=(no_copy_ctor&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

struct no_copy_assign : base {
    no_copy_assign& operator=(no_copy_assign&) = delete;

    no_copy_assign() : base() {
        EXT_FUNCTION_NAME
        ++default_ctor;
    }

    no_copy_assign(base b) : base(b) {
        EXT_FUNCTION_NAME
        ++non_default_ctor;
    }

    no_copy_assign(no_copy_assign const& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++copy_ctor;
    }

    no_copy_assign(no_copy_assign&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_ctor;
    }

    no_copy_assign& operator=(no_copy_assign&& other) {
        EXT_FUNCTION_NAME
        copy_values(&other);
        ++move_assign;
        return *this;
    }
};

}}}    // namespace ext::util::tracing
#endif // EXT_UTIL_TRACING_CLASSES_HEADER
