#include <vector>
#include <map>
#include <iostream>
#include <obi/util/show.hpp>
#include <tuple>
#include <obi/meta/tuple.hpp>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using obi::util::operator<<;

    // define data
    using combined = obi::meta::tuple_cat_t<std::tuple<int,float,void>
                                          ,std::tuple<char,double>
                                          >;

    std::cout << obi::meta::tuple_size<combined>::value << std::endl;

    std::tuple<int,int,int> tuple1 = {1,2,3};
    auto print = [](auto a, int b, int c){ std::cout << a << " "
                                                     << b << " "
                                                     << c << " "
                                                     << std::endl; };
    obi::meta::tuple_apply(tuple1, print, 23, 42);


    return 0;
}

