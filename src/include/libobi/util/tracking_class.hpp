// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include <iostream>

namespace obi { namespace util {

    template<class T>
    class tracking_class {
        T _value;

    public:
        //default constructor
        tracking_class() : _value() {
            std::cout << "Default Constructor:             " << _value << std::endl;
        }

        //copy constructor
        tracking_class(T v) : _value(v) {
            std::cout << "Copy Constructor, value:         " << _value << std::endl;
        }

        tracking_class(const tracking_class& other) : _value(other._value) {
            std::cout << "Copy Constructor, value:         " << _value << std::endl;
        }

        //move constructor
        tracking_class(tracking_class&& other) : _value(std::move(other._value)) {
            std::cout << "Move Constructor, value:         " << _value << std::endl;
        }

        tracking_class(tracking_class const&& other) = delete;

        //destructor
        ~tracking_class() {
            std::cout << "Destructor, value:               " << _value << std::endl;
        }

        //member
        T set_value(T const v) : _value(v) {
            std::cout << "Constructor, value:              " << _value << std::endl;
        }

        tracking_class& operator= (tracking_class&& other) {
            if(this != &other)
                _value = std::move(other._value);
            std::cout << "move assign, value               " << _value << std::endl;
        }

        tracking_class& operator= (tracking_class const& other) {
            if(this != &other)
                _value = other._value;
            std::cout << "copy assign, value               " << _value << std::endl;
        }
    };
}}  // namespace obi::util
