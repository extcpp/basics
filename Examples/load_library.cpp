#include <iostream>
#include <libobi/util/load_library.hpp>

int main(){
    using namespace std;
    using namespace obi::util;
    string lib("/usr/lib/libmpi.so");
    string symbol("mca_base_open");
    string symbol_fail("get_string_from_object");

    cout << "now trying to open:" << lib << endl;
    auto handle = dl_open(lib);
    if ( handle == NULL){
        cout << "handle is NULL" << endl;
    } else {
        cout << "handle is " << (long) handle << endl;
    }
    auto symbol_address      = dl_sym(handle, symbol);
    auto symbol_address_fail = dl_sym(handle, symbol_fail);
    dl_close(handle);
    return 0;
}
