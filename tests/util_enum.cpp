// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/enum.hpp>
#include <ext/macros/compiler.hpp>
#include <gtest/gtest.h>

namespace eu = ::ext::util;

#ifndef EXT_COMPILER_VC
TEST(util_enum, is_fixed_enum) {
    enum class enum_a { };
    enum class enum_b : uint8_t { };
    enum enum_c { };
    enum enum_d : uint8_t { };

    static_assert(eu::is_fixed_enum_v<enum_a>);
    static_assert(eu::is_fixed_enum_v<enum_b>);
    static_assert(!eu::is_fixed_enum_v<enum_c>);
    static_assert(eu::is_fixed_enum_v<enum_d>);
}
#endif

TEST(util_enum, enum_to_underlying) {
    enum class enum_a : uint8_t { one = 1, two = 2, three = 3 };

    ASSERT_EQ(eu::enum_to_underlying(enum_a::one), 1);
    ASSERT_EQ(eu::enum_to_underlying_safe<uint8_t>(enum_a::one), 1);
}

TEST(util_enum, underlying_to_enum) {
    enum class enum_a : int { one = 1, two = 2, three = 3 };
    enum class enum_b : long { one = 1, two = 2, three = 3 };

    ASSERT_EQ(eu::underlying_to_enum<enum_a>(2), enum_a::two);
    ASSERT_EQ(eu::underlying_to_enum<enum_b>(2L), enum_b::two);

    ASSERT_EQ(eu::underlying_to_enum_unsafe<enum_a>(2L), enum_a::two);
}

TEST(util_enum, enum_to_enum) {
    enum class enum_a : int { one = 1, two = 2, three = 3 };
    enum class enum_b : int { one = 1, two = 2, three = 3 };
    enum class enum_c : long { one = 1, two = 2, three = 3 };

    // enums should be sub / supser sets of each other
    ASSERT_EQ((eu::enum_to_enum<enum_b>(enum_a::three)), (enum_b::three));

    // enums are of same underlying types
    ASSERT_EQ((eu::enum_to_enum_unsafe<enum_c>(enum_a::three)), (enum_c::three));
}
