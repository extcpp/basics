#include <array>
#include <type_traits>
#include "../meta/pack.hpp"

namespace obi { namespace structures {

    //custom types (via move)
    template <typename T, std::size_t N>
    class vector_base_user_defined {
    public:
        vector_base_user_defined() = default;
        template <typename Arg, typename... Args, typename = std::enable_if_t<sizeof...(Args)+1 == N>>
        vector_base_user_defined(Arg&& arg, Args&&... args) : //at leaset one arg
            _values {std::forward<Arg>(arg), std::forward<Args>(args)...}
        {}
    private:
        std::array<T, N> _values;
    };

    //build-in sse types etc
    template <typename T, std::size_t N, typename>
    class vector_base_arithmetic;
    template <typename T, std::size_t N, typename... Args>
    class vector_base_arithmetic<T, N, obi::meta::pack<Args...>> {
    public:
        vector_base_arithmetic() = default;
        vector_base_arithmetic(Args... args) : _values{{args...}} {}
    private:
        std::array<T, N> _values;
    };


    template <typename T, std::size_t N>
    class vector : std::conditional_t<std::is_arithmetic<T>::value,
                                      vector_base_arithmetic<T, N, typename ::obi::meta::generate_pack<N, T>::type>,
                                      vector_base_user_defined<T, N>>{

        using base = std::conditional_t<std::is_arithmetic<T>::value,
                                        vector_base_arithmetic<T, N, typename ::obi::meta::generate_pack<N, T>::type>,
                                        vector_base_user_defined<T, N>
                                       >;
        using base::base;
    };

}}  // obi::structures
