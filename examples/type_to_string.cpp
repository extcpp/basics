// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <obi/util/type_to_string.hpp>
#include <obi/macros/platform.hpp>

using namespace obi::util;

int main()
{
    int i(3);
    double d(3);
    const std::string s("foo");
    const double& dr = d;

#if !defined(OBI_WINDOWS) || _MSC_VER >= 1920
    OBI_TYPE_PRINT(i);
    OBI_TYPE_PRINT(d);
    OBI_TYPE_PRINT(dr);
    OBI_TYPE_PRINT(s);
#endif

    return 0;
}
