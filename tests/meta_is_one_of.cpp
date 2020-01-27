// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/meta/is_one_of.hpp>
#include <gtest/gtest.h>

using namespace std;
using namespace ext::meta;

TEST(meta_is_one_of, is_one_of) {
    ASSERT_TRUE((is_one_of_v<int, double, float, int>) );
    ASSERT_TRUE((is_one_of_v<int, double, int, float>) );
    ASSERT_FALSE((is_one_of_v<int, double, float>) );
    ASSERT_FALSE((is_one_of_v<int>) );
}
