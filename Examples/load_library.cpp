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
        cout << "handle is " << reinterpret_cast<long>(handle) << endl;
    }

    auto symbol_address      = dl_sym(handle, symbol);
    if ( symbol_address == NULL){
        cout << "could not get symbol" << endl;
        cout << dl_error() << endl;
    } else {
        cout << "symbol is at " << reinterpret_cast<long>(symbol_address) << endl;
    }

    try{
        symbol_address = dl_sym_e(handle, symbol_fail);
        cout << "symbol is at " << reinterpret_cast<long>(symbol_address) << endl;
    } catch (logic_error e){
        cout << e.what() << endl;
    }

    dl_close(handle);

    return 0;
}
