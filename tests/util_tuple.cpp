// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/macros/compiler.hpp>
#if __cplusplus <= 201703L
EXT_WARNING("skipping build of tuple test because of old compiler")
#else

    #include <ext/util/tuple.hpp>
    #include <gtest/gtest.h>
    #include <string>

TEST(util_tuple, tuple_for_each) {
    std::tuple<int, int, int> tup(1, 2, 3);
    std::vector<int> result;
    ext::util::tuple_for_each(tup, [&result](int x) {
        result.push_back(x);
    });
    ASSERT_EQ(result, (std::vector<int>{1, 2, 3}));
}

#endif // __cplusplus <= 201703L
