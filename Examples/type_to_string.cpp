#include <libobi/util.hpp>

using namespace obi::util;

int main()
{
    int i(3);
    double d(3);
    const std::string s("foo");
    const double& dr = d;

    OBI_TYPE_PRINT(i);
    OBI_TYPE_PRINT(d);
    OBI_TYPE_PRINT(dr);
    OBI_TYPE_PRINT(s);

    return 0;
}
