// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#include "../meta/has_member.hpp"
#include "container_helper.hpp"
#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>

namespace obi { namespace util {

    template <typename Container> inline
    //if it is a container, that is mentioned in my container traits
    std::enable_if_t<obi::meta::has_category_member<_detail::container_traits<Container>>::value
                        && !std::is_same<typename   _detail::container_traits<Container>::category ,_detail::associative_tag>::value
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

    template <typename Key, typename Value> inline
    std::ostream& out_pair_in_map (std::ostream &out, const std::pair<Key, Value>& pair) {
        using namespace std;
        out << pair.first << "=" << pair.second;
        return out;
    }

    template <typename Container> inline
    std::enable_if_t<obi::meta::has_category_member<_detail::container_traits<Container>>::value
                        && std::is_same<typename    _detail::container_traits<Container>::category ,_detail::associative_tag>::value
                    ,std::ostream&
                    >
    operator<< (std::ostream &out, const Container& container) {
        using namespace std;
        out << "{";
        if (container.size() == 1) {
            out_pair_in_map(out, *container.begin());
        }
        if (container.size() > 1) {
            auto current = container.begin();
            while (next(current) != container.end()) {
                out_pair_in_map(out, *current) << ",";
                current++;
            }
            out_pair_in_map(out, *current);

        }
        out << "}";
        return out;
    }

    template <typename Key, typename Value> inline
    std::ostream& operator<< (std::ostream &out, const std::pair<Key, Value>& pair) {
        using namespace std;
        out << "(" << pair.first << "," << pair.second << ")";
        return out;
    }


    template <typename T> inline
    std::string show(T item){
        std::stringstream ss;
        ss << item;
        return ss.str();
    }

}}
