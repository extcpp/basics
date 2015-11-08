// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include <iostream>

namespace obi { namespace util {

    template<class T>
    class tracking_class {
        T _value;
        static unsigned const _width = 28;

    public:
        //default constructor
        tracking_class() : _value(){
            std::cout << std::setw(_width) <<
                "Default Constructor:" << _value << std::endl;
        }

        //copy constructor
        tracking_class(T v) : _value(v){
            std::cout << std::setw(_width) <<
                "Copy Constructor, value:" << _value << std::endl;
        }

        tracking_class(const tracking_class& other) : _value(other._value){
            std::cout << std::setw(_width) <<
                "Copy Constructor, value:" << _value << std::endl;
        }

        //move constructor
        tracking_class(tracking_class&& other) : _value(std::move(other._value)){
            std::cout << std::setw(_width) <<
                "Move Constructor, value:" << _value << std::endl;
        }

        tracking_class(tracking_class const&& other) = delete;

        //destructor
        ~tracking_class(){
            std::cout << std::setw(_width) <<
                "Destructor, value:" << _value << std::endl;
        }

        //member
        T set_value(T const v){
            _value = v;
            std::cout << std::setw(_width) <<
                "Constructor, value:" << _value << std::endl;
        }

        tracking_class& operator= (tracking_class&& other){
            if(this != &other){
                _value = std::move(other._value);
            }
            std::cout << std::setw(_width) <<
                "move assign, value:" << _value << std::endl;
        }

        tracking_class& operator= (tracking_class const& other){
            if(this != &other){
                _value = other._value;
            }
            std::cout << std::setw(_width) <<
                "copy assign, value" << _value << std::endl;
        }
    };
}}  // namespace obi::util
