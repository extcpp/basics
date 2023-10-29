// Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics

#include <gtest/gtest.h>
#define EXT_DEFER_ENABLE_MOVE_ASSIGN
#define EXT_DEFER_ALLOW_NON_NOTHROW_INVOCABLE
#include <ext/util/defer.hpp>
#include <functional>
#include <type_traits>

using namespace ext::util;
using namespace std::literals;

static int defer_may_throw_x = 0;
void defer_may_throw_free_function() {
    defer_may_throw_x++;
    throw std::logic_error{"nobody will see this"};
} // deprecation warning should be generated

TEST(util_defer_may_throw, special) {
    defer lambda([&]() {
        throw std::logic_error{" intentionally broken "};
    }); // deprecation warning should be generated

    static_assert(!std::is_copy_constructible_v<decltype(lambda)>);
    static_assert(!std::is_copy_assignable_v<decltype(lambda)>);
    static_assert(std::is_move_assignable_v<decltype(lambda)>);

    static_assert(!std::is_copy_constructible_v<decltype(defer_may_throw_free_function)>);
    static_assert(
        !std::is_assignable_v<decltype(defer_may_throw_free_function), decltype(defer_may_throw_free_function)>);
    static_assert(!std::is_move_assignable_v<decltype(defer_may_throw_free_function)>);
}

TEST(util_defer_may_throw, move_assign_lambda) {
    int a = 0;
    {
        defer action1(std::function<void()>([&]() noexcept {
            a++;
        })); // std function can not be noexcept?
        defer action2(std::function<void()>([&]() noexcept {
            a += 2;
        }));
        action1 = std::move(action2);
        ASSERT_EQ(a, 0);
    }
    ASSERT_EQ(a, 2);
}

TEST(util_defer_may_throw, move_assing) {
    defer_may_throw_x = 0;
    {
        defer action1(&defer_may_throw_free_function);
        defer action2(&defer_may_throw_free_function);
        action1 = std::move(action2);
        ASSERT_EQ(defer_may_throw_x, 0);
    }
    ASSERT_EQ(defer_may_throw_x, 1);
}
