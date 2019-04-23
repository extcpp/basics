// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_ALGORITHM_BASIC_HEADER
#define OBI_ALGORITHM_BASIC_HEADER

#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>
#include "../meta/has_member.hpp"

namespace obi { namespace algorithm {

    template<typename T, typename Predicate = std::less<>>
    T const& min(T const& a, T const& b, T const& c, Predicate comp = Predicate()){
        return std::min(a,std::min(b, c, comp), comp);
    }

    template<typename T, typename Predicate = std::greater<>>
    T const& max(T const& a, T const& b, T const& c, Predicate comp = Predicate()){
        return std::max(std::max(a, b, comp), c, comp);
    }


    // count occurences in vector like containers /////////////////////////////////////////
    template<typename Iterator, typename Int = std::size_t>
    std::map<typename std::iterator_traits<Iterator>::value_type,Int>
    count_occurrences(Iterator begin, Iterator end) {
        using Key = typename std::iterator_traits<Iterator>::value_type;
        std::map<Key,Int> result;
        for(auto it = begin; it != end; it++) {
                result[*it]++;
        }
        return result;
    }

    //add enable if for vector like containers
    template<typename Container, typename Int = int>
    auto count_occurrences(const Container& container) {
        using Iterator = decltype(std::begin(container));
        return count_occurrences<Iterator,Int>(std::begin(container), std::end(container));
    }


    // mege maps //////////////////////////////////////////////////////////////
    // items that are already in the map get replaced by a later insert if
    // the predicate evaluates to true
    template <typename Map, typename Predicate = std::less<>>
    auto& merge_maps(Map& result, const Map& in, Predicate predicate = Predicate() ) {
        for (auto it = in.begin(); it != in.end(); it++) {
            auto found = result.find(it->first);
            if(found != result.end()){
                if (predicate(found->second,it->second)){
                    result[it->first] = it->second;
                }
            } else {
                result[it->first] = it->second;
            }
        }
        return result;
    }

    //looks like maps are given in a iterator range
    template <typename Iterator
             ,typename Predicate = std::less<>
             ,typename = std::enable_if_t<obi::meta::is_input_iterator<Iterator>::value>
             >
    auto merge_maps(Iterator begin, Iterator end, Predicate predicate = Predicate()) {
        using PairType = typename std::iterator_traits<Iterator>::value_type;
        std::map<typename PairType::T1,typename PairType::T2> result;
        for (auto it = begin; it != end; it++) {
            merge_maps(result, *it, predicate);
        }
        return result;
    }

}} // obi::algorithm
#endif // OBI_ALGORITHM_BASIC_HEADER
