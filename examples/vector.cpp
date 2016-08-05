#include <obi/structures/vector.hpp>

int main() {
    double d = 10.0;
    #pragma message "the following warining about narrowing is expected!"
    //::obi::structures::vector <float, 3> x = {d, 2.0f, 3.0f}; //does not build with clang
    //(void) x;
    (void) d;
}
