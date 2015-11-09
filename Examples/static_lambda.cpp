#include <obi/meta/static_lambda.hpp>
#include <iostream>

using namespace std;
using namespace obi::meta;

int main() {

    const constexpr auto add_one = OBI_STATIC_LAMBDA(auto x)
    {
            return x + 1;
    };

    std::cout << "1 + 1 = " << add_one(1) << endl;
}
