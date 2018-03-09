#include <obi/meta.hpp>
#include <iostream>

//using namespace std;
using namespace obi::meta;

struct leet {
        using type = void;
        static const int value = 1337;
};

struct unleet {};

int main() {

    std::cout << "is_one_of<int,double,float,int>(): "
         <<  is_one_of<int,double,float,int>()
         <<  std::endl;
    std::cout << "is_one_of<int,double,float>():     "
         <<  is_one_of<int,double,float>()
         <<  std::endl;

    std::cout << "is_one_of<void_t<int>,int,void>():     "
         <<  is_one_of<void_t<int>,int,void>()
         <<  std::endl;

    std::cout << "has_value_var<leet>()     "
         <<  has_var_value_v<leet>
         <<  std::endl;

    std::cout << "has_type_member<unleet>()     "
         <<  has_type_type_v<unleet>
         <<  std::endl;

    return 0;
}
