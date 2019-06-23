#include <ext/meta/if.hpp>
#include <gtest/gtest.h>

using namespace std;
using namespace ext::meta;

TEST(meta_if, if_) {
    ASSERT_TRUE((std::is_same_v<if_t<std::true_type, double, int>, double>) );
    ASSERT_TRUE((std::is_same_v<if_t<std::false_type, double, int>, int>) );
}

TEST(meta_if, if_c) {
    ASSERT_TRUE((std::is_same_v<if_c_t<true, double, int>, double>) );
    ASSERT_TRUE((std::is_same_v<if_c_t<false, double, int>, int>) );
}
