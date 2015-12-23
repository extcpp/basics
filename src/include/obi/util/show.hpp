#include <iostream>
#include <type_traits>
#include "container_helper.hpp"
#include "../meta/has_member.hpp"
#include <string>
#include <sstream>

namespace obi { namespace util {

    template <typename Container>
    //if it is a container, that is mentioned in my container traits
    std::enable_if_t<obi::meta::has_category_member<_detail::container_traits<Container>>::value
                    ,std::ostream&
                    >
    operator<< (std::ostream &out, const Container& container) {
        using namespace std;
        out << "[";
        if (container.size() == 1) {
            out <<  *container.begin();
        }
        if (container.size() > 1) {
            auto current = container.begin();
            while (next(current) != container.end()) {
                out << *current << ",";
                current++;
            }
            out << *current;

        }
        out << "]";
        return out;
    }

    template <typename Key, typename Value>
    std::ostream& operator<< (std::ostream &out, const std::pair<Key, Value>& pair) {
        using namespace std;
        out << "(" << pair.first << "," << pair.second << ")";
        return out;
    }

    template <typename T>
    std::string show(T item){
        std::stringstream ss;
        ss << item;
        return ss.str();
    }

}}
