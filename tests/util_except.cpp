#include <cstring>
#include <ext/util/except.hpp>
#include <gtest/gtest.h>

using namespace std::literals;

TEST(except, just_throw) {
    using namespace ext::util;

    ASSERT_THROW(throw not_implemented_exception(), ext::util::not_implemented_exception);
    ASSERT_THROW(throw debug_exception("somehting happend"), ext::util::debug_exception);
    ASSERT_THROW(throw permission_denied_exception("not allowed to access file"),
                 ext::util::permission_denied_exception);
    ASSERT_THROW(throw cat_not_connect_exception("to storage"), ext::util::cat_not_connect_exception);
}
