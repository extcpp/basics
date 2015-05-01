// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include <iostream>

namespace obi { namespace util {

    template<class T>
    class tracking_class
    {
        T _value;
    public:
        tracking_class() : _value() {
            std::cout << "Default Constructor:             " << _value << std::endl;
        }
        tracking_class(T v) : _value(v){
            std::cout << "Constructor, value:              " << _value << std::endl;
        }
        tracking_class(const tracking_class& other) : _value(other._value){
            std::cout << "Copy Constructor, value:         " << _value << std::endl;
        }
        tracking_class(tracking_class&& other) {
            _value = std::move(other._value);
            std::cout << "Move Constructor, value:         " << _value << std::endl;
        }
        ~tracking_class(){
            std::cout << "Destructor, value:       " << _value << std::endl;
        }
        T set_value(T const value){
            _value=value;
        }
    };
}}  // namespace obi::util
