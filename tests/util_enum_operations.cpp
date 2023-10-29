// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/enum_operations.hpp>
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

const std::uint8_t ui8_0000 = 0b0000'0000;
const std::uint8_t ui8_0001 = 0b0000'0001;
const std::uint8_t ui8_0010 = 0b0000'0010;
const std::uint8_t ui8_0011 = 0b0000'0011;
const std::uint8_t ui8_0100 = 0b0000'0100;
const std::uint8_t ui8_0101 = 0b0000'0101;
const std::uint8_t ui8_0110 = 0b0000'0110;
const std::uint8_t ui8_0111 = 0b0000'0111;
const std::uint8_t ui8_1000 = 0b0000'1000;
const std::uint8_t ui8_1001 = 0b0000'1001;
const std::uint8_t ui8_1110 = 0b1111'1110;
const std::uint8_t ui8_1111 = 0b1111'1111;


TEST(util_enum_operations, op_and) {
    using namespace eu::operators::enumeration;

    const auto x = (test::enum_0010 & test::enum_0001);
    static_assert(std::is_same_v<const test, decltype(x)>);
    ASSERT_TRUE(x == test::enum_0000);


    ASSERT_TRUE((test::enum_0001 & test::enum_0010) == test::enum_0000);
    ASSERT_TRUE((test::enum_0010 & test::enum_0001) == test::enum_0000);
    ASSERT_TRUE((test::enum_0001 & test::enum_0011) == test::enum_0001);
    ASSERT_TRUE((test::enum_0011 & test::enum_0001) == test::enum_0001);

    ASSERT_TRUE((ui8_0001 & test::enum_0010) == test::enum_0000);
    ASSERT_TRUE((test::enum_0010 & ui8_0001) == test::enum_0000);
    ASSERT_TRUE((ui8_0001 & test::enum_0011) == test::enum_0001);
    ASSERT_TRUE((test::enum_0011 & ui8_0001) == test::enum_0001);

    std::uint8_t u;
    test e;

    u = ui8_0001;
    u &= test::enum_0011;
    ASSERT_TRUE(u == ui8_0001);

    e = test::enum_0001;
    e &= ui8_0011;
    ASSERT_TRUE(e == test::enum_0001);

    e = test::enum_0001;
    e &= test::enum_0011;
    ASSERT_TRUE(e == test::enum_0001);
}

TEST(util_enum_operations, op_or) {
    using namespace eu::operators::enumeration;

    const auto x = (test::enum_0010 | test::enum_0001);
    static_assert(std::is_same_v<const test, decltype(x)>);
    ASSERT_TRUE(x == test::enum_0011);

    ASSERT_TRUE((test::enum_0001 | test::enum_0010) == test::enum_0011);
    ASSERT_TRUE((test::enum_0010 | test::enum_0001) == test::enum_0011);
    ASSERT_TRUE((test::enum_0001 | test::enum_0011) == test::enum_0011);
    ASSERT_TRUE((test::enum_0011 | test::enum_0001) == test::enum_0011);

    ASSERT_TRUE((ui8_0001 | test::enum_0010) == test::enum_0011);
    ASSERT_TRUE((test::enum_0010 | ui8_0001) == test::enum_0011);
    ASSERT_TRUE((ui8_0001 | test::enum_0011) == test::enum_0011);
    ASSERT_TRUE((test::enum_0011 | ui8_0001) == test::enum_0011);

    std::uint8_t u;
    test e;

    u = ui8_0001;
    u |= test::enum_0011;
    ASSERT_TRUE(u == ui8_0011);

    e = test::enum_0001;
    e |= ui8_0011;
    ASSERT_TRUE(e == test::enum_0011);

    e = test::enum_0001;
    e |= test::enum_0011;
    ASSERT_TRUE(e == test::enum_0011);
}

TEST(util_enum_operations, op_xor) {
    using namespace eu::operators::enumeration;

    const auto x = (test::enum_0010 ^ test::enum_0001);
    static_assert(std::is_same_v<const test, decltype(x)>);
    ASSERT_TRUE(x == test::enum_0011);

    ASSERT_TRUE((test::enum_0001 ^ test::enum_0010) == test::enum_0011);
    ASSERT_TRUE((test::enum_0010 ^ test::enum_0001) == test::enum_0011);
    ASSERT_TRUE((test::enum_0001 ^ test::enum_0011) == test::enum_0010);
    ASSERT_TRUE((test::enum_0011 ^ test::enum_0001) == test::enum_0010);

    ASSERT_TRUE((ui8_0001 ^ test::enum_0010) == test::enum_0011);
    ASSERT_TRUE((test::enum_0010 ^ ui8_0001) == test::enum_0011);
    ASSERT_TRUE((ui8_0001 ^ test::enum_0011) == test::enum_0010);
    ASSERT_TRUE((test::enum_0011 ^ ui8_0001) == test::enum_0010);

    std::uint8_t u;
    test e;

    u = ui8_0001;
    u ^= test::enum_0011;
    ASSERT_TRUE(u == ui8_0010);

    e = test::enum_0001;
    e ^= ui8_0011;
    ASSERT_TRUE(e == test::enum_0010);

    e = test::enum_0001;
    e ^= test::enum_0011;
    ASSERT_TRUE(e == test::enum_0010);
}

TEST(util_enum_operations, op_not) {
    using namespace eu::operators::enumeration;

    const auto x = ~test::enum_0001;
    static_assert(std::is_same_v<const test, decltype(x)>);
    ASSERT_TRUE(x == test::enum_1110);

    ASSERT_TRUE(~test::enum_1111 == test::enum_0000);
    ASSERT_TRUE(~test::enum_0001 == test::enum_1110);
}
