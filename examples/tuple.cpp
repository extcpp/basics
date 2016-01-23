#include <vector>
#include <map>
#include <iostream>
#include <obi/util/show.hpp>
#include <obi/meta/tuple.hpp>

int main(/*int argc, const char *argv[]*/) {
    // pull in operator
    using obi::util::operator<<;

    // define data
    using combined = obi::meta::tuple_cat_t<std::tuple<int,float,void>
                                          ,std::tuple<char,double>
                                          >;

    std::cout << obi::meta::tuple_size<combined>::value << std::endl;
    return 0;
}

