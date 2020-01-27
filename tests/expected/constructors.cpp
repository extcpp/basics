// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include "expected.hpp"
#include <string>
#include <type_traits>
#include <vector>

struct takes_init_and_variadic {
    std::vector<int> v;
    std::tuple<int, int> t;
    template<class... Args>
    takes_init_and_variadic(std::initializer_list<int> l, Args&&... args) : v(l), t(std::forward<Args>(args)...) {}
};

TEST(expected_constructors, constructors) {
    {
        eu::expected<int, int> e;
        EXPECT_TRUE(e);
        EXPECT_TRUE(e == 0);
    }

    {
        eu::expected<int, int> e = eu::make_unexpected(0);
        EXPECT_TRUE(!e);
        EXPECT_TRUE(e.error() == 0);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 0);
        EXPECT_TRUE(!e);
        EXPECT_TRUE(e.error() == 0);
    }

    {
        eu::expected<int, int> e(std::in_place, 42);
        EXPECT_TRUE(e);
        EXPECT_TRUE(e == 42);
    }

    {
        eu::expected<std::vector<int>, int> e(std::in_place, {0, 1});
        EXPECT_TRUE(e);
        EXPECT_TRUE((*e)[0] == 0);
        EXPECT_TRUE((*e)[1] == 1);
    }

    {
        eu::expected<std::tuple<int, int>, int> e(std::in_place, 0, 1);
        EXPECT_TRUE(e);
        EXPECT_TRUE(std::get<0>(*e) == 0);
        EXPECT_TRUE(std::get<1>(*e) == 1);
    }

    {
        eu::expected<takes_init_and_variadic, int> e(std::in_place, {0, 1}, 2, 3);
        EXPECT_TRUE(e);
        EXPECT_TRUE(e->v[0] == 0);
        EXPECT_TRUE(e->v[1] == 1);
        EXPECT_TRUE(std::get<0>(e->t) == 2);
        EXPECT_TRUE(std::get<1>(e->t) == 3);
    }

    {
        eu::expected<int, int> e;
        EXPECT_TRUE((std::is_default_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_trivially_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_trivially_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_trivially_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<int, std::string> e;
        EXPECT_TRUE((std::is_default_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<std::string, int> e;
        EXPECT_TRUE((std::is_default_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<std::string, std::string> e;
        EXPECT_TRUE((std::is_default_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((std::is_move_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        EXPECT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<void, int> e;
        EXPECT_TRUE(e);
    }

    {
        eu::expected<void, int> e(eu::unexpect, 42);
        EXPECT_TRUE(!e);
        EXPECT_TRUE(e.error() == 42);
    }
}
