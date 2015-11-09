#include <iostream>
#include <obi/util/aascopes.hpp>
#include <stdexcept>

int main()
{
    try {
        std::cout << "enter scope 1" << std::endl;
        OBI_SCOPE_EXIT      { std::cout << "exit" << std::endl; };
        OBI_SCOPE_FAIL      { std::cout << "fail" << std::endl; };
        throw std::logic_error("error for testing");
        std::cout << "exit scope 1" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "catched - " << e.what() << std::endl;
    }
    {
        std::cout << "enter scope 2" << std::endl;
        OBI_SCOPE_SUCCESS { std::cout << "success" << std::endl; };
        std::cout << "exit scope 2" << std::endl;
    }
    return 0;
}
