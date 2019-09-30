#include <ext/util/enum.hpp>
#include <gtest/gtest.h>

namespace eu = ::ext::util;

enum class testenum : int {
    // clang-format off
    one    = 0b0001,
    two    = 0b0010,
    three  = 0b0011,
    four   = 0b0100,
    five   = 0b0101,
    six    = 0b0110,
    seven  = 0b0111,
    eight  = 0b1000,
    nine   = 0b1001
    // clang-format on
};

using eu::operator|;
using eu::operator|=;
using eu::operator&;
using eu::operator&=;

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

TEST(util_enum, enum_to_underlying) {
    enum class enum_a : int { one = 1, two = 2, three = 3 };

    ASSERT_EQ(eu::enum_to_underlying_unsafe(enum_a::one), 1);
    ASSERT_EQ(eu::enum_to_underlying<int>(enum_a::one), 1);
}

TEST(util_enum, underlying_to_enum) {
    enum class enum_a : int { one = 1, two = 2, three = 3 };

    enum class enum_b : long { one = 1, two = 2, three = 3 };

    ASSERT_EQ(eu::underlying_to_enum<enum_a>(2), enum_a::two);
    ASSERT_EQ(eu::underlying_to_enum<enum_b>(2L), enum_b::two);
}

TEST(util_enum, enum_to_enum) {
    enum class enum_a : int { one = 1, two = 2, three = 3 };

    enum class enum_b : int { one = 1, two = 2, three = 3 };

    enum class enum_c : long { one = 1, two = 2, three = 3 };

    ASSERT_EQ(eu::enum_to_enum<enum_b>(enum_a::three), enum_b::three);
}

