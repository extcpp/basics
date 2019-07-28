#include <ext/util/enum.hpp>
#include <gtest/gtest.h>

namespace ou = ::ext::util;

enum class testenum : int {
    one    = 0b0001,
    two    = 0b0010,
    three  = 0b0011,
    four   = 0b0100,
    five   = 0b0101,
    six    = 0b0110,
    seven  = 0b0111,
    eight  = 0b1000,
    nine   = 0b1001
};

using ou::operator|;
using ou::operator|=;
using ou::operator&;
using ou::operator&=;

TEST(util_enum, or) {

    ASSERT_EQ(testenum::three, testenum::one | testenum::two);
    {
        auto x = testenum::one;
        x |= testenum::two;
        ASSERT_EQ(testenum::three, x);
    }

    ASSERT_EQ(testenum::three, testenum::one | 2);
    {
        auto x = testenum::one;
        x |= 2;
        ASSERT_EQ(testenum::three, x);
    }

}

TEST(util_enum, and) {

    ASSERT_EQ(testenum::one, testenum::three & testenum::one);
    {
        auto x = testenum::three;
        x &= testenum::one;
        ASSERT_EQ(testenum::one, x);
    }

    ASSERT_EQ(testenum::one, testenum::three & 1);
    {
        auto x = testenum::three;
        x &= 1;
        ASSERT_EQ(testenum::one, x);
    }

}
