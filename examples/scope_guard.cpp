#include <iostream>
#include <obi/util/scope_guard.hpp>
#include <stdexcept>

void fun(){ std::cout << "fun" << std::endl; };
struct functor{
    void operator()(){
        std::cout << "functor " << std::endl;
    };
};

int main()
{
    ::obi::util::scope_guard{fun};
    ::obi::util::scope_guard{&fun};

    auto lambda_fun = [](){ std::cout << "lambda_fun" << std::endl; };
    ::obi::util::scope_guard{lambda_fun};
    ::obi::util::scope_guard{std::move(lambda_fun)};

    functor func;
    ::obi::util::scope_guard{func};
    ::obi::util::scope_guard{std::move(func)};

    std::cout << std::endl;

    try {
        std::cout << "enter scope 1" << std::endl;
        OBI_SCOPE_EXIT    { std::cout << "exit callback" << std::endl; };
        OBI_SCOPE_SUCCESS { std::cout << "success callback" << std::endl; };
        OBI_SCOPE_FAIL    { std::cout << "fail callback" << std::endl; };
        throw std::logic_error("error for example");
        std::cout << "exit scope 1" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    try {
        std::cout << "enter scope 2" << std::endl;
        OBI_SCOPE_EXIT    { std::cout << "exit callback" << std::endl; };
        OBI_SCOPE_SUCCESS { std::cout << "success callback" << std::endl;
                            throw std::logic_error ("throw during success callback");
                          };
        OBI_SCOPE_FAIL    { std::cout << "fail callback" << std::endl; };
        std::cout << "exit scope 2" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    try {
        std::cout << "enter scope 3" << std::endl;
        OBI_SCOPE_EXIT    { std::cout << "exit callback" << std::endl;
                            throw std::logic_error ("throw during exit callback");
                          };
        OBI_SCOPE_SUCCESS { std::cout << "success callback" << std::endl; };
        OBI_SCOPE_FAIL    { std::cout << "fail callback" << std::endl; };
        std::cout << "exit scope 3" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught - " << e.what() << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
