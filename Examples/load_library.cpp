#include <libobi/util/load_library.hpp>

int main(){
    std::string lib("/usr/lib/libmpi.so");
    obi::util::dl_open(lib);
    return 0;
}
