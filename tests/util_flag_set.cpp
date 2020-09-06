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


enum class ec : uint8_t{
    none = 0b0000,
    foo  = 0b0001,
    bar  = 0b0010,
    baz  = 0b0100
};

void is_flags_enum(ec){} // REVIEW -- is not enough to enable operators
EXT_ENABLE_FLAG_SET_OPERATORS(ec)



using namespace eu;
TEST(util_flag_set, layout) {
    static_assert(std::is_standard_layout_v<flag_set<ec>>);
    ASSERT_FALSE(std::is_trivial_v<flag_set<ec>>); // flags are initalized to 0
    ASSERT_FALSE(std::is_pod_v<flag_set<ec>>); // not trivial
}

TEST(util_flag_set, contains) {
    {
        flag_set<ec> set;
        ASSERT_TRUE(set.contains(ec::none));
        ASSERT_FALSE(set.contains(ec::foo));
        ASSERT_FALSE(set.contains(ec::bar));
        ASSERT_FALSE(set.contains(ec::baz));
    }

    {
        flag_set<ec> set = ec::foo;
        ASSERT_TRUE(set.contains(ec::none)); // x & 0 == 0 (always true)
        ASSERT_TRUE(set.contains(ec::foo));
        ASSERT_FALSE(set.contains(ec::bar));
        ASSERT_FALSE(set.contains(ec::baz));
    }

    {
        // requires enabled operators
        flag_set<ec> set = ec::foo | ec::bar;
        ASSERT_TRUE(set.contains(ec::none));
        ASSERT_TRUE(set.contains(ec::foo));
        ASSERT_TRUE(set.contains(ec::bar));
        ASSERT_FALSE(set.contains(ec::baz));
    }
}

TEST(util_flag_set, equals) {
    {
        flag_set<ec> set;
        ASSERT_TRUE (set == ec::none);
        ASSERT_FALSE(set == ec::foo);
        ASSERT_FALSE(set == ec::bar);
        ASSERT_FALSE(set == ec::baz);
    }

    {
        flag_set<ec> set = ec::foo;
        ASSERT_FALSE(set == ec::none);
        ASSERT_TRUE (set == ec::foo);
        ASSERT_FALSE(set == ec::bar);
        ASSERT_FALSE(set == ec::baz);
    }

    {
        // requires enabled operators
        flag_set<ec> set = ec::foo | ec::bar;
        ASSERT_FALSE(set == ec::none);
        ASSERT_FALSE(set == ec::foo);
        ASSERT_FALSE(set == ec::bar);
        ASSERT_FALSE(set == ec::baz);
        ASSERT_TRUE (set == (ec::foo | ec::bar));
    }
}

TEST(util_flag_set, not_equals) {
    {
        flag_set<ec> set;
        ASSERT_FALSE(set != ec::none);
        ASSERT_TRUE (set != ec::foo);
        ASSERT_TRUE (set != ec::bar);
        ASSERT_TRUE (set != ec::baz);
    }

    {
        flag_set<ec> set = ec::foo;
        ASSERT_TRUE (set != ec::none);
        ASSERT_FALSE(set != ec::foo);
        ASSERT_TRUE (set != ec::bar);
        ASSERT_TRUE (set != ec::baz);
    }

    {
        // requires enabled operators
        flag_set<ec> set = ec::foo | ec::bar;
        ASSERT_TRUE (set != ec::none);
        ASSERT_TRUE (set != ec::foo);
        ASSERT_TRUE (set != ec::bar);
        ASSERT_TRUE (set != ec::baz);
        ASSERT_FALSE(set != (ec::foo | ec::bar));
    }
}

TEST(util_flag_set, create) {
    using fs = flag_set<ec>;
    {
        fs set = ec::foo | ec::bar;
        ASSERT_TRUE (set.flags == fs(ec::foo | ec::bar).flags);
    }

    {
        fs set = ec::none;
        ASSERT_TRUE (set.flags == fs().flags);
    }

    {
        fs set;
        ASSERT_TRUE (set.flags == fs().flags);
    }
}

TEST(util_flag_set, add) {
    flag_set<ec> set;
    set.add(ec::foo);
    ASSERT_TRUE (set == ec::foo);
    set.add(ec::bar);
    ASSERT_TRUE (set == (ec::foo | ec::bar));
}

TEST(util_flag_set, remove) {
    flag_set<ec> set = (ec::foo | ec::bar);
    set.remove(ec::foo);
    ASSERT_TRUE (set == ec::bar);
    set.remove(ec::bar);
    ASSERT_TRUE (set == ec::none);
}


TEST(util_flag_set, copy) {
    eu::flag_set<ec> set = ec::foo;

    ASSERT_TRUE(set == ec::foo);

    flag_set<ec> set2(set);
    ASSERT_TRUE(set2 == ec::foo);

    flag_set<ec> set3;
    set3 = set2;
    ASSERT_TRUE(set3 == ec::foo);

    flag_set<ec> set4(std::move(set3));
    ASSERT_TRUE(set4 == ec::foo);

    flag_set<ec> set5;
    set5 = std::move(set4);
    ASSERT_TRUE(set5 == ec::foo);
}


TEST(util_flag_set, op_and) {
    using namespace eu;
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

TEST(util_flag_set, op_or) {
    using namespace eu;

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

TEST(util_flag_set, op_xor) {
    using namespace eu;

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

TEST(util_flag_set, op_not) {
    using namespace eu;

    const auto two_fifty_five = (~test::enum_0000);
    static_assert(std::is_same_v<const flag_set<test>, decltype(two_fifty_five)>);
    ASSERT_TRUE(two_fifty_five == flag_1111);

    ASSERT_TRUE(~test::enum_0000 == flag_1111);
    ASSERT_TRUE(~test::enum_0001 == flag_1110);

    ASSERT_TRUE(~flag_1111 == flag_0000);
    ASSERT_TRUE(~flag_1110 == flag_0001);
}
