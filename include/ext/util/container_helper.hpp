// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Idea and code from STL
// http://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-3-of-n
#pragma once
#ifndef EXT_UTIL_CONTAINER_HELPER_HEADER
#    define EXT_UTIL_CONTAINER_HELPER_HEADER

#    include <algorithm>
#    include <deque>
#    include <forward_list>
#    include <list>
#    include <map>
#    include <set>
#    include <unordered_map>
#    include <unordered_set>
#    include <vector>

// TODO - finish array

namespace ext { namespace util {
namespace _detail {

// possilbe categorys for containers
struct arraylike_tag {};
struct vectorlike_tag {};
struct listlike_tag {};
struct associative_tag {};

// MTF that returns the category of a container
template<typename C>
struct container_traits;

// array like
template<typename T, std::size_t N>
struct container_traits<std::array<T, N>> {
    typedef arraylike_tag category;
};

// vector like
template<typename T, typename A>
struct container_traits<std::vector<T, A>> {
    typedef vectorlike_tag category;
};
template<typename T, typename A>
struct container_traits<std::deque<T, A>> {
    typedef vectorlike_tag category;
};

// list like
template<typename T, typename A>
struct container_traits<std::list<T, A>> {
    typedef listlike_tag category;
};
template<typename T, typename A>
struct container_traits<std::forward_list<T, A>> {
    typedef listlike_tag category;
};

// associative
template<typename T, typename C, typename A>
struct container_traits<std::set<T, C, A>> {
    typedef associative_tag category;
};
template<typename T, typename C, typename A>
struct container_traits<std::multiset<T, C, A>> {
    typedef associative_tag category;
};
template<typename T, typename C, typename A>
struct container_traits<std::unordered_set<T, C, A>> {
    typedef associative_tag category;
};
template<typename T, typename C, typename A>
struct container_traits<std::unordered_multiset<T, C, A>> {
    typedef associative_tag category;
};
template<typename K, typename V, typename C, typename A>
struct container_traits<std::map<K, V, C, A>> {
    typedef associative_tag category;
};
template<typename K, typename V, typename C, typename A>
struct container_traits<std::multimap<K, V, C, A>> {
    typedef associative_tag category;
};
template<typename K, typename V, typename C, typename A>
struct container_traits<std::unordered_map<K, V, C, A>> {
    typedef associative_tag category;
};
template<typename K, typename V, typename C, typename A>
struct container_traits<std::unordered_multimap<K, V, C, A>> {
    typedef associative_tag category;
};

////tag dispatching
// vector like
template<typename Container, typename X>
void erase_helper(Container& c, const X& x, vectorlike_tag) {
    c.erase(std::remove(c.begin(), c.end(), x), c.end());
}
template<typename Container, typename Predicate>
void erase_if_helper(Container& c, Predicate p, vectorlike_tag) {
    c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
}
template<typename Container, class Compare> // template template would fuck up
// allocators!!!
void sort_helper_comp(Container& c, Compare comp, vectorlike_tag) {
    std::sort(c.begin(), c.end(), comp);
}
template<typename Container> // So I rather wirte this twice:P
void sort_helper_comp(Container& c, vectorlike_tag) {
    std::sort(c.begin(), c.end());
}
// list like
template<typename Container, typename X>
void erase_helper(Container& c, const X& x, listlike_tag) {
    c.remove(x);
}
template<typename Container, typename Predicate>
void erase_if_helper(Container& c, Predicate p, listlike_tag) {
    c.remove_if(p);
}
template<typename Container, class Compare>
void sort_helper_comp(Container& c, Compare comp, listlike_tag) {
    c.sort(comp);
}
template<typename Container>
void sort_helper(Container& c, listlike_tag) {
    c.sort();
}
// associative
template<typename Container, typename X>
void erase_helper(Container& c, const X& x, associative_tag) {
    c.erase(x);
}
template<typename Container, typename Predicate>
void erase_if_helper(Container& c, Predicate p, associative_tag) {
    for (auto i = c.begin(); i != c.end();) {
        if (p(*i)) {
            c.erase(i++);
        } else {
            ++i;
        }
    }
}

//  NOTE
//  I like the dispatching way below more than STL's way.
//  But it is not as good extensible so I follow his
//  suggestion and avoid additional member functions:)
//  But passing empty structs as Arguments is kind
//  of awkward as well:( - The optimizer will prevail!

//  //dispatch category
//  template<typename Category>
//  struct category_dispatcher;

//  template<>
//  struct category_dispatcher<vectorlike_tag> {
//      template <typename Container, typename X>
//      static void erase_helper(Container& c, const X& x) {
//          c.erase(std::remove(c.begin(), c.end(), x), c.end());
//      }

//      template <typename Container, typename Predicate>
//      static void erase_if_helper(Container& c, Predicate p) {
//          c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
//      }
//  };

//  template<>
//  struct category_dispatcher<listlike_tag> {
//      template <typename Container, typename X>
//      static void erase_helper(Container& c, const X& x) {
//          c.remove(x);
//      }

//      template <typename Container, typename Predicate>
//      static void erase_if_helper(Container& c, Predicate p) {
//          c.remove_if(p);
//      }
//  };

//  template<>
//  struct category_dispatcher<associative_tag> {
//      template <typename Container, typename X>
//      static void erase_helper(Container& c, const X& x) {
//          c.erase(x);
//      }

//      template <typename Container, typename Predicate>
//      static void erase_if_helper(Container& c, Predicate p) {
//          for (auto i = c.begin(); i != c.end(); ) {
//              if (p(*i)) {
//                  c.erase(i++);
//              } else {
//                  ++i;
//              }
//          }
//      }
//  };

} // namespace _detail

// dispatch tags
// erase
template<typename Container, typename X>
void erase(Container& c, const X& x) {
    _detail::erase_helper(c, x, typename _detail::container_traits<Container>::category());
}
template<typename Container, typename Predicate>
void erase_if(Container& c, Predicate p) {
    _detail::erase_if_helper(c, p, typename _detail::container_traits<Container>::category());
}

// sort
template<typename Container, typename Compare>
void sort(Container& c, Compare comp) {
    _detail::sort_helper_comp(c, comp, typename _detail::container_traits<Container>::category());
}
template<typename Container>
void sort(Container& c) {
    _detail::sort_helper(c, typename _detail::container_traits<Container>::category());
}
//  template <typename Container, typename X>
//  void erase(Container& c, const X& x) {
//      typedef typename _detail::container_traits<Container>::category
//      category;
//      _detail::category_dispatcher<category>::erase_helper(c, x);
//  }

//  template <typename Container, typename Predicate>
//  void erase_if(Container& c, Predicate p) {
//      typedef typename _detail::container_traits<Container>::category
//      category;
//      _detail::category_dispatcher<category>::erase_if_helper(c, p);
//  }
}}     // namespace ext::util
#endif // EXT_UTIL_CONTAINER_HELPER_HEADER
