// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <ext/util/type_to_string.hpp>
#include <ext/macros/platform.hpp>

using namespace ext::util;

int main()
{
    int i(3);
    double d(3);
    const std::string s("foo");
    const double& dr = d;

#if !defined(EXT_WINDOWS) || _MSC_VER >= 1920
    EXT_TYPE_PRINT(i);
    EXT_TYPE_PRINT(d);
    EXT_TYPE_PRINT(dr);
    EXT_TYPE_PRINT(s);
#endif // !defined(EXT_WINDOWS) || _MSC_VER >= 1920

    return 0;
}
