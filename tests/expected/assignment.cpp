// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include "expected.hpp"

TEST(expected_assignment, simple) {
    eu::expected<int, int> e1 = 42;
    eu::expected<int, int> e2 = 17;
    eu::expected<int, int> e3 = 21;
    eu::expected<int, int> e4 = eu::make_unexpected(42);
    eu::expected<int, int> e5 = eu::make_unexpected(17);
    eu::expected<int, int> e6 = eu::make_unexpected(21);

    e1 = e2;
    EXPECT_TRUE(e1);
    EXPECT_TRUE(*e1 == 17);
    EXPECT_TRUE(e2);
    EXPECT_TRUE(*e2 == 17);

    e1 = std::move(e2);
    EXPECT_TRUE(e1);
    EXPECT_TRUE(*e1 == 17);
    EXPECT_TRUE(e2);
    EXPECT_TRUE(*e2 == 17);

    e1 = 42;
    EXPECT_TRUE(e1);
    EXPECT_TRUE(*e1 == 42);

    auto unex = eu::make_unexpected(12);
    e1 = unex;
    EXPECT_TRUE(!e1);
    EXPECT_TRUE(e1.error() == 12);

    e1 = eu::make_unexpected(42);
    EXPECT_TRUE(!e1);
    EXPECT_TRUE(e1.error() == 42);

    e1 = e3;
    EXPECT_TRUE(e1);
    EXPECT_TRUE(*e1 == 21);

    e4 = e5;
    EXPECT_TRUE(!e4);
    EXPECT_TRUE(e4.error() == 17);

    e4 = std::move(e6);
    EXPECT_TRUE(!e4);
    EXPECT_TRUE(e4.error() == 21);

    e4 = e1;
    EXPECT_TRUE(e4);
    EXPECT_TRUE(*e4 == 21);
}

TEST(expected_assignment, deletion) {
    struct has_all {
        has_all() = default;
        has_all(const has_all&) = default;
        has_all(has_all&&) noexcept = default;
        has_all& operator=(const has_all&) = default;
    };

    eu::expected<has_all, has_all> e1 = {};
    eu::expected<has_all, has_all> e2 = {};
    e1 = e2;

    struct except_move {
        except_move() = default;
        except_move(const except_move&) = default;
        except_move(except_move&&) noexcept(false){};
        except_move& operator=(const except_move&) = default;
    };
    eu::expected<except_move, except_move> e3 = {};
    eu::expected<except_move, except_move> e4 = {};

    (void) e3;
    (void) e4;
    // e3 = e4; //should not compile
}
