#include <iostream>
#include <obi/util/scope_guard.hpp>
#include <stdexcept>

int main()
{
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


    auto lambda_fun = [](){ return "dfsdf"; };

    ::obi::util::scope_guard{lambda_fun};
    return 0;
}
