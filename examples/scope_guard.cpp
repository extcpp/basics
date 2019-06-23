// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/scope_guard.hpp>
#include <iostream>
#include <stdexcept>

void fun() {
    std::cout << "fun" << std::endl;
}
struct functor {
    void operator()() {
        std::cout << "functor " << std::endl;
    };
};

int main() {
    ::ext::util::scope_guard{fun};
    ::ext::util::scope_guard{&fun};

    auto lambda_fun = []() { std::cout << "lambda_fun" << std::endl; };
    ::ext::util::scope_guard{lambda_fun};
    ::ext::util::scope_guard{std::move(lambda_fun)};

    functor func;
    ::ext::util::scope_guard{func};
    ::ext::util::scope_guard{std::move(func)};

    std::cout << std::endl;

    try {
        std::cout << "enter scope 1" << std::endl;
        EXT_SCOPE_EXIT {
            std::cout << "exit callback" << std::endl;
        };
        EXT_SCOPE_SUCCESS {
            std::cout << "success callback" << std::endl;
        };
        EXT_SCOPE_FAIL {
            std::cout << "fail callback" << std::endl;
        };
        throw std::logic_error("error for example");
        std::cout << "exit scope 1" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    try {
        std::cout << "enter scope 2" << std::endl;
        EXT_SCOPE_EXIT {
            std::cout << "exit callback" << std::endl;
        };
        EXT_SCOPE_SUCCESS {
            std::cout << "success callback" << std::endl;
            throw std::logic_error("throw during success callback");
        };
        EXT_SCOPE_FAIL {
            std::cout << "fail callback" << std::endl;
        };
        std::cout << "exit scope 2" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    try {
        std::cout << "enter scope 3" << std::endl;
        // will result in termination
        EXT_SCOPE_EXIT {
            std::cout << "exit callback" << std::endl;
            throw std::logic_error("throw during exit callback");
        };
        EXT_SCOPE_SUCCESS {
            std::cout << "success callback" << std::endl;
        };
        EXT_SCOPE_FAIL {
            std::cout << "fail callback" << std::endl;
        };
        std::cout << "exit scope 3" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
