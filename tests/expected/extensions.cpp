// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include "expected.hpp"

#define TOKENPASTE(x, y) x##y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define STATIC_REQUIRE(e)                            \
    constexpr bool TOKENPASTE2(rqure, __LINE__) = e; \
    EXPECT_TRUE(e);

TEST(expected_extensions, map) {
    auto mul2 = [](int a) {
        return a * 2;
    };
    auto ret_void = [](int a) {
        (void) a;
    };

    {
        eu::expected<int, int> e = 21;
        auto ret = e.map(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.map(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).map(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).map(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = e.map(ret_void);
        EXPECT_TRUE(ret);
        // STATIC_REQUIRE((std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.map(ret_void);
        EXPECT_TRUE(ret);
        // STATIC_REQUIRE( (std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).map(ret_void);
        EXPECT_TRUE(ret);
        // STATIC_REQUIRE( (std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).map(ret_void);
        EXPECT_TRUE(ret);
        // STATIC_REQUIRE( (std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map(ret_void);
        EXPECT_TRUE(!ret);
        // STATIC_REQUIRE( (std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map(ret_void);
        EXPECT_TRUE(!ret);
        // STATIC_REQUIRE( (std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map(ret_void);
        EXPECT_TRUE(!ret);
        // STATIC_REQUIRE((std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map(ret_void);
        EXPECT_TRUE(!ret);
        // STATIC_REQUIRE((std::is_same<decltype(ret), eu::expected<void, int>>::value));
    }


    // mapping functions which return references
    {
        eu::expected<int, int> e(42);
        auto ret = e.map([](int& i) -> int& {
            return i;
        });
        EXPECT_TRUE(ret);
        EXPECT_TRUE(ret == 42);
    }
}

TEST(expected_extensions, map_error) {
    auto mul2 = [](int a) {
        return a * 2;
    };
    auto ret_void = [](int a) {
        (void) a;
    };

    {
        eu::expected<int, int> e = 21;
        auto ret = e.map_error(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.map_error(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).map_error(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).map_error(mul2);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map_error(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 42);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map_error(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 42);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map_error(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 42);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map_error(mul2);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 42);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = e.map_error(ret_void);
        EXPECT_TRUE(ret);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.map_error(ret_void);
        EXPECT_TRUE(ret);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).map_error(ret_void);
        EXPECT_TRUE(ret);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).map_error(ret_void);
        EXPECT_TRUE(ret);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map_error(ret_void);
        EXPECT_TRUE(!ret);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.map_error(ret_void);
        EXPECT_TRUE(!ret);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map_error(ret_void);
        EXPECT_TRUE(!ret);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).map_error(ret_void);
        EXPECT_TRUE(!ret);
    }
}

TEST(expected_extensions, and_then) {
    auto succeed = [](int a) {
        (void) a;
        return eu::expected<int, int>(21 * 2);
    };
    auto fail = [](int a) {
        (void) a;
        return eu::expected<int, int>(eu::unexpect, 17);
    };

    {
        eu::expected<int, int> e = 21;
        auto ret = e.and_then(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.and_then(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).and_then(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).and_then(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = e.and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.and_then(succeed);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.and_then(succeed);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).and_then(succeed);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).and_then(succeed);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).and_then(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }
}

TEST(expected_extensions, or_else) {
    using eptr = std::unique_ptr<int>;
    auto succeed = [](int a) {
        (void) a;
        return eu::expected<int, int>(21 * 2);
    };
    auto succeedptr = [](eptr e) {
        (void) e;
        return eu::expected<int, eptr>(21 * 2);
    };
    auto fail = [](int a) {
        (void) a;
        return eu::expected<int, int>(eu::unexpect, 17);
    };
    auto efail = [](eptr e) {
        *e = 17;
        return eu::expected<int, eptr>(eu::unexpect, std::move(e));
    };
    // auto failptr = [](eptr e) { return eu::expected<int,eptr>(eu::unexpect, std::move(e));};
    auto failvoid = [](int) {
    };
    auto failvoidptr = [](const eptr&) { /* don't consume */ };
    auto consumeptr = [](eptr) {
    };
    auto make_u_int = [](int n) {
        return std::unique_ptr<int>(new int(n));
    };

    {
        eu::expected<int, int> e = 21;
        auto ret = e.or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, eptr> e = 21;
        auto ret = std::move(e).or_else(succeedptr);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = e.or_else(fail);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = e.or_else(fail);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e = 21;
        auto ret = std::move(e).or_else(fail);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(ret == 21);
    }


    {
        eu::expected<int, eptr> e = 21;
        auto ret = std::move(e).or_else(efail);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(ret == 21);
    }

    {
        const eu::expected<int, int> e = 21;
        auto ret = std::move(e).or_else(fail);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, eptr> e(eu::unexpect, make_u_int(21));
        auto ret = std::move(e).or_else(succeedptr);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(succeed);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(*ret == 42);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(failvoid);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = e.or_else(failvoid);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(failvoid);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }

    {
        eu::expected<int, eptr> e(eu::unexpect, make_u_int(21));
        auto ret = std::move(e).or_else(failvoidptr);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(*ret.error() == 21);
    }

    {
        eu::expected<int, eptr> e(eu::unexpect, make_u_int(21));
        auto ret = std::move(e).or_else(consumeptr);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == nullptr);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(fail);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 17);
    }

    {
        const eu::expected<int, int> e(eu::unexpect, 21);
        auto ret = std::move(e).or_else(failvoid);
        EXPECT_TRUE(!ret);
        EXPECT_TRUE(ret.error() == 21);
    }
}
struct S {
    int x;
};

struct F {
    int x;
};

TEST(expected_extensions, issue14) {
    auto res = eu::expected<S, F>{eu::unexpect, F{}};
    res.map_error([](F f) {
        (void) f;
    });
}

TEST(expected_extensions, issue32) {
    int i = 0;
    eu::expected<void, int> a;
    a.map([&i] {
        i = 42;
    });
    EXPECT_TRUE(i == 42);

    auto x = a.map([] {
        return 42;
    });
    EXPECT_TRUE(*x == 42);
}
