// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/flag_set.hpp>
#include <gtest/gtest.h>

namespace eu = ::ext::util;

enum class test : uint8_t {
    // clang-format off
    enum_0000 = 0b0000'0000,
    enum_0001 = 0b0000'0001,
    enum_0010 = 0b0000'0010,
    enum_0011 = 0b0000'0011,
    enum_0100 = 0b0000'0100,
    enum_0101 = 0b0000'0101,
    enum_0110 = 0b0000'0110,
    enum_0111 = 0b0000'0111,
    enum_1000 = 0b0000'1000,
    enum_1001 = 0b0000'1001,
    enum_1110 = 0b1111'1110,
    enum_1111 = 0b1111'1111
    // clang-format on
};

EXT_ENABLE_FLAG_SET_OPERATORS(test)

const eu::flag_set<test> flag_0000 = test::enum_0000;
const eu::flag_set<test> flag_0001 = test::enum_0001;
const eu::flag_set<test> flag_0010 = test::enum_0010;
const eu::flag_set<test> flag_0011 = test::enum_0011;
const eu::flag_set<test> flag_1110 = test::enum_1110;
const eu::flag_set<test> flag_1111 = test::enum_1111;

TEST(util_flag_set, flag_set_values ) {
    ASSERT_EQ(flag_0000.flags, 0);
    ASSERT_EQ(flag_0001.flags, 1);
    ASSERT_EQ(flag_0010.flags, 2);
    ASSERT_EQ(flag_0011.flags, 3);
    ASSERT_EQ(flag_1110.flags, 254);
    ASSERT_EQ(flag_1111.flags, 255);
}


TEST(util_flag_set, flag_set_and ) {
    using namespace eu::operators::flag_set;

    const auto one = (test::enum_0011 & test::enum_0001);
    static_assert(std::is_same_v<const flag_set<test>, decltype(one)>);
    ASSERT_TRUE(one == flag_0001);

    ASSERT_TRUE((test::enum_0001 & test::enum_0010) == flag_0000);
    ASSERT_TRUE((test::enum_0010 & test::enum_0001) == flag_0000);
    ASSERT_TRUE((test::enum_0001 & test::enum_0011) == flag_0001);
    ASSERT_TRUE((test::enum_0011 & test::enum_0001) == flag_0001);

    ASSERT_TRUE((flag_0001 & flag_0010) == flag_0000);
    ASSERT_TRUE((flag_0010 & flag_0001) == flag_0000);
    ASSERT_TRUE((flag_0001 & flag_0011) == flag_0001);
    ASSERT_TRUE((flag_0011 & flag_0001) == flag_0001);

    ASSERT_TRUE((flag_0001 & test::enum_0010) == flag_0000);
    ASSERT_TRUE((test::enum_0010 & flag_0001) == flag_0000);
    ASSERT_TRUE((flag_0001 & test::enum_0011) == flag_0001);
    ASSERT_TRUE((test::enum_0011 & flag_0001) == flag_0001);

    flag_set<test> x;

    x = flag_0001;
    x &= flag_0010;
    ASSERT_TRUE(x == flag_0000);

    x = flag_0001;
    x &= flag_0011;
    ASSERT_TRUE(x == flag_0001);

    x = flag_0001;
    x &= test::enum_0010;
    ASSERT_TRUE(x == flag_0000);

    x = flag_0001;
    x &= test::enum_0011;
    ASSERT_TRUE(x == flag_0001);
}

TEST(util_flag_set, flag_set_or ) {
    using namespace eu::operators::flag_set;

    const auto three = (test::enum_0011 | test::enum_0001);
    static_assert(std::is_same_v<const flag_set<test>, decltype(three)>);
    ASSERT_TRUE(three == flag_0011);

    ASSERT_TRUE((test::enum_0001 | test::enum_0010) == flag_0011);
    ASSERT_TRUE((test::enum_0010 | test::enum_0001) == flag_0011);
    ASSERT_TRUE((test::enum_0001 | test::enum_0011) == flag_0011);
    ASSERT_TRUE((test::enum_0011 | test::enum_0001) == flag_0011);

    ASSERT_TRUE((flag_0001 | flag_0010) == flag_0011);
    ASSERT_TRUE((flag_0010 | flag_0001) == flag_0011);
    ASSERT_TRUE((flag_0001 | flag_0011) == flag_0011);
    ASSERT_TRUE((flag_0011 | flag_0001) == flag_0011);

    ASSERT_TRUE((flag_0001 | test::enum_0010) == flag_0011);
    ASSERT_TRUE((test::enum_0010 | flag_0001) == flag_0011);
    ASSERT_TRUE((flag_0001 | test::enum_0011) == flag_0011);
    ASSERT_TRUE((test::enum_0011 | flag_0001) == flag_0011);

    flag_set<test> x;

    x  = flag_0001;
    x |= flag_0010;
    ASSERT_TRUE(x == flag_0011);

    x  = flag_0001;
    x |= flag_0011;
    ASSERT_TRUE(x == flag_0011);

    x  = flag_0001;
    x |= test::enum_0010;
    ASSERT_TRUE(x == flag_0011);

    x  = flag_0001;
    x |= test::enum_0011;
    ASSERT_TRUE(x == flag_0011);
}

TEST(util_flag_set, flag_set_xor ) {
    using namespace eu::operators::flag_set;

    const auto two = (test::enum_0011 ^ test::enum_0001);
    static_assert(std::is_same_v<const flag_set<test>, decltype(two)>);
    ASSERT_TRUE(two == flag_0010);

    ASSERT_TRUE((test::enum_0001 ^ test::enum_0010) == flag_0011);
    ASSERT_TRUE((test::enum_0010 ^ test::enum_0001) == flag_0011);
    ASSERT_TRUE((test::enum_0001 ^ test::enum_0011) == flag_0010);
    ASSERT_TRUE((test::enum_0011 ^ test::enum_0001) == flag_0010);

    ASSERT_TRUE((flag_0001 ^ flag_0010) == flag_0011);
    ASSERT_TRUE((flag_0010 ^ flag_0001) == flag_0011);
    ASSERT_TRUE((flag_0001 ^ flag_0011) == flag_0010);
    ASSERT_TRUE((flag_0011 ^ flag_0001) == flag_0010);

    ASSERT_TRUE((flag_0001 ^ test::enum_0010) == flag_0011);
    ASSERT_TRUE((test::enum_0010 ^ flag_0001) == flag_0011);
    ASSERT_TRUE((flag_0001 ^ test::enum_0011) == flag_0010);
    ASSERT_TRUE((test::enum_0011 ^ flag_0001) == flag_0010);

    flag_set<test> x;

    x  = flag_0001;
    x ^= flag_0010;
    ASSERT_TRUE(x == flag_0011);

    x  = flag_0001;
    x ^= flag_0011;
    ASSERT_TRUE(x == flag_0010);

    x  = flag_0001;
    x ^= test::enum_0010;
    ASSERT_TRUE(x == flag_0011);

    x  = flag_0001;
    x ^= test::enum_0011;
    ASSERT_TRUE(x == flag_0010);
}

TEST(util_flag_set, flag_set_not ) {
    using namespace eu::operators::flag_set;

    const auto two_fifty_five = (~test::enum_0000);
    static_assert(std::is_same_v<const flag_set<test>, decltype(two_fifty_five)>);
    ASSERT_TRUE(two_fifty_five == flag_1111);

    ASSERT_TRUE(~test::enum_0000 == flag_1111);
    ASSERT_TRUE(~test::enum_0001 == flag_1110);

    ASSERT_TRUE(~flag_1111 == flag_0000);
    ASSERT_TRUE(~flag_1110 == flag_0001);
}
