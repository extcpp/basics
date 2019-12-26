#include <ext/meta/if_all_any.hpp>
#include <gtest/gtest.h>

using namespace std;
using namespace ext::meta;

TEST(meta_if_all_any, if_all) {
    ASSERT_TRUE((if_all<true>()));
    ASSERT_TRUE((if_all<true, true>()));
    ASSERT_FALSE((if_all<true, false, true>()));
    ASSERT_FALSE((if_all_v<false>) );
}

TEST(meta_if_all_any, if_any) {
    ASSERT_TRUE((if_any<true>()));
    ASSERT_TRUE((if_any<true, true>()));
    ASSERT_TRUE((if_any<true, false, true>()));
    ASSERT_FALSE((if_any_v<false>) );
    ASSERT_FALSE((if_any_v<false, false, false>) );
}
