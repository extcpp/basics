#include <obi/memory/tagged_pointer.hpp>
#include <obi/util/encode.hpp>

using namespace std;
using namespace obi;

int main() {

    uint64_t value = 8;
    auto* int_pointer = &value;

    memory::tagged_pointer pointer(int_pointer, 5);

    cout << "        int_pointer: " << int_pointer << endl;
    cout << "      pointer.get(): " << pointer.get() << endl;
    cout << "      pointer.tag(): " << pointer.tag() << endl;
    cout << "      pointer.tag(): " << util::encode_binary(pointer.tag()) << endl;
    cout << "       pointer.mask: " << pointer.mask << endl;
    cout << "       pointer.mask: " << util::encode_binary(pointer.mask) << endl;
    cout << "             is pod: " << std::is_pod_v<decltype(pointer)> << endl;
    cout << "has standard layout: " << std::is_standard_layout_v<decltype(pointer)> << endl;

    return 0;
}
