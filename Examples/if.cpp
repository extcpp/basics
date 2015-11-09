#include <obi/meta/if.hpp>
#include <iostream>

using namespace std;
using namespace obi::meta;


int main() {

    cout << "\nnext\n"
         <<  if_t<std::true_type,double,int>(1.5)
         << "\nnext\n"
         <<  if_t<std::false_type,double,int>(1.5)
         << "\nnext\n"
         <<  if_c_t<true,double,int>(1.5)
         << "\nnext\n"
         <<  if_c_t<false,double,int>(1.5)
         <<  endl;

    return 0;
}
