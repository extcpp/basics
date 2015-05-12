#include <libobi/meta.hpp>
#include <iostream>

using namespace std;
using namespace obi::meta;

struct leet {
        using type = void;
        static const int value = 1337;
};

int main() {

    cout << "is_one_of<int,double,float,int>(): "
         <<  is_one_of<int,double,float,int>()
         <<  endl;
    cout << "is_one_of<int,double,float>():     "
         <<  is_one_of<int,double,float>()
         <<  endl;

    cout << "is_one_of<void_t<int>,int,void>():     "
         <<  is_one_of<void_t<int>,int,void>()
         <<  endl;

    cout << "has_value_var<leet>()     "
         <<  has_value_var<leet>()
         <<  endl;

    cout << "has_type_member<leet>()     "
         <<  has_type_member<leet>()
         <<  endl;

    return 0;
}
