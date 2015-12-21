#include <iostream>
#include <type_traits>
#include "container_helper.hpp"
#include "../meta/void_t.hpp"
#include <string>
#include <sstream>

namespace obi { namespace util {

    //check for category type
    template<class, class = void>
    struct has_category_member : std::false_type {};

    template<class T>
    struct has_category_member<T, ::obi::meta::void_t<typename T::category> > : std::true_type {};

    template <typename Container>
    //if it is a container, that is mentioned in my container traits
    std::enable_if_t<has_category_member<_detail::container_traits<Container>>::value
                    ,std::ostream&
                    >
    operator<< (std::ostream &out, const Container& container) {
        using namespace std;
        out << "[ ";
        if (container.size() == 1) {
            out <<  *container.begin() << " ";
        }
        if (container.size() > 1) {
            auto current = container.begin();
            while (next(current) != container.end()) {
                out << *current << ", ";
                current++;
            }
            out << *current << " ";

        }
        out << "]";
        return out;
    }

    template <typename T>
    std::string show(T item){
        std::stringstream ss;
        ss << item;
        return ss.str();
    }

}}
