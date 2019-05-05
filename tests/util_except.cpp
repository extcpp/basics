#include <gtest/gtest.h>
#include <obi/util/except.hpp>
#include <cstring>

using namespace std::literals;


TEST(except, just_throw){
    using namespace obi::util;

    ASSERT_THROW(throw not_implemented_exception(), obi::util::not_implemented_exception);
    ASSERT_THROW(throw debug_exception("somehting happend"), obi::util::debug_exception);
    ASSERT_THROW(throw permission_denied_exception("not allowed to access file"), obi::util::permission_denied_exception);
    ASSERT_THROW(throw cat_not_connect_exception("to storage"), obi::util::cat_not_connect_exception);
}

