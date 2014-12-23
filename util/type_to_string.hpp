#include <utility>
#include <type_traits>
#include <memory>
#include <iostream>

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

namespace obi { namespace util {

template<class T> std::string
type_to_string()
//! create string providing description of given type
{
    using DT = typename std::decay<T>::type ; // DT - DecayedType
                      //std::remove_reference
    #ifdef __GNUC__
    // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
    int status{};
    std::unique_ptr<char, void(*)(void*)> demangeled_name(
        abi::__cxa_demangle(typeid(DT).name(),
                            nullptr, /*output buffer*/
                            nullptr, /*bufferlen*/
                            &status
        ),
        std::free // deleter - void free(void* ptr)
    );

    switch (status)
    {
        case -1:
            return "Error - demangle: a memory allocation failiure occurred.";
            break;
        case -2:
            return "Error - demangle: mangled_name is not a valid name under the c++ abi mangling rules.";
            break;
        case -3:
            return "Error - demangle: One of the arguments is invalid.";
            break;
    }

    std::string type_name = demangeled_name.get();
    #else
    std::string type_name = typeid(DT).name();
    #endif

    std::string result;

    if (std::is_const<T>::value)
        result += "const ";
    if (std::is_volatile<T>::value)
        result += "volatile ";

    result += type_name;

    if (std::is_lvalue_reference<T>::value)
        result += "&";
    else if (std::is_rvalue_reference<T>::value)
        result += "&&";
    return result;
}

//turn param to "string"
#define OBI_TO_STRING(X) #X

//create string that describes type of given var
#define OBI_TYPE_STRING(X) obi::util::type_to_string<decltype(X)>()

//print type of given var
#define OBI_TYPE_PRINT(X) do {\
    std::cout << "Variable " OBI_TO_STRING(X) " is of type: " \
              << obi::util::type_to_string<decltype(X)>() << std::endl;  \
    } while(0);

}}  // namespace obi::util
