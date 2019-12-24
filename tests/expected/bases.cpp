#include "expected.hpp"
#include <string>

TEST(expected_bases, trivially) {
    ASSERT_TRUE((std::is_trivially_copy_constructible_v<eu::expected<int, int>>) );
    ASSERT_TRUE((std::is_trivially_copy_assignable<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_trivially_move_constructible<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_trivially_move_assignable<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_trivially_destructible<eu::expected<int, int>>::value));

    ASSERT_TRUE((std::is_trivially_copy_constructible<eu::expected<void, int>>::value));
    ASSERT_TRUE((std::is_trivially_move_constructible<eu::expected<void, int>>::value));
    ASSERT_TRUE((std::is_trivially_destructible<eu::expected<void, int>>::value));


    {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };
        ASSERT_TRUE((std::is_trivially_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_trivially_copy_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_trivially_move_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_trivially_move_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_trivially_destructible<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(const T&) {}
            T(T&&){};
            T& operator=(const T&) {
                return *this;
            }
            T& operator=(T&&) {
                return *this;
            }
            ~T() {}
        };
        ASSERT_TRUE((!std::is_trivially_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_trivially_copy_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_trivially_move_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_trivially_move_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_trivially_destructible<eu::expected<T, int>>::value));
    }
}

TEST(expected_bases, deletion) {
    ASSERT_TRUE((std::is_copy_constructible<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_copy_assignable<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_move_constructible<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_move_assignable<eu::expected<int, int>>::value));
    ASSERT_TRUE((std::is_destructible<eu::expected<int, int>>::value));

    {
        struct T {
            T() = default;
        };
        ASSERT_TRUE((std::is_default_constructible<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(int) {}
        };
        ASSERT_TRUE((!std::is_default_constructible<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };
        ASSERT_TRUE((std::is_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_copy_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_move_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_move_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_destructible<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(const T&) = delete;
            T(T&&) = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&) = delete;
        };
        ASSERT_TRUE((!std::is_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_copy_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_move_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_move_assignable<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(const T&) = delete;
            T(T&&) = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&) = default;
        };
        ASSERT_TRUE((!std::is_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((!std::is_copy_assignable<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_move_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_move_assignable<eu::expected<T, int>>::value));
    }

    {
        struct T {
            T(const T&) = default;
            T(T&&) = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&) = delete;
        };
        ASSERT_TRUE((std::is_copy_constructible<eu::expected<T, int>>::value));
        ASSERT_TRUE((std::is_copy_assignable<eu::expected<T, int>>::value));
    }

    {
        eu::expected<int, int> e;
        ASSERT_TRUE((std::is_default_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_trivially_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_trivially_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_trivially_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<int, std::string> e;
        ASSERT_TRUE((std::is_default_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<std::string, int> e;
        ASSERT_TRUE((std::is_default_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }

    {
        eu::expected<std::string, std::string> e;
        ASSERT_TRUE((std::is_default_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((std::is_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((std::is_move_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_copy_assignable<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_constructible<decltype(e)>::value));
        ASSERT_TRUE((!std::is_trivially_move_assignable<decltype(e)>::value));
    }
}
