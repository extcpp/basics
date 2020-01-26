// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include "expected.hpp"

#include <string>

using std::string;

eu::expected<int, string> getInt3(int val) {
    return val;
}

eu::expected<int, string> getInt2(int val) {
    return val;
}

eu::expected<int, string> getInt1() {
    return getInt2(5).and_then(getInt3);
}

TEST(expected_extensions, issue1) {
    getInt1();
}

eu::expected<int, int> operation1() {
    return 42;
}

eu::expected<std::string, int> operation2(int const val) {
    return "Bananas";
}

TEST(expected_extensions, issue17) {
    auto const intermediate_result = operation1();

    intermediate_result.and_then(operation2);
}

struct a {};
struct b : a {};

auto doit() -> eu::expected<std::unique_ptr<b>, int> {
    return eu::make_unexpected(0);
}

TEST(expected_extensions, issue23) {
    eu::expected<std::unique_ptr<a>, int> msg = doit();
    EXPECT_TRUE(!msg.has_value());
}

TEST(expected_extensions, issue26) {
    eu::expected<a, int> exp = eu::expected<b, int>(eu::unexpect, 0);
    EXPECT_TRUE(!exp.has_value());
}

struct foo {
    foo() = default;
    foo(foo&) = delete;
    foo(foo&&){};
};

TEST(expected_extensions, issue29) {
    std::vector<foo> v;
    v.emplace_back();
    eu::expected<std::vector<foo>, int> ov = std::move(v);
    EXPECT_TRUE(ov->size() == 1);
}

eu::expected<int, std::string> error() {
    return eu::make_unexpected(std::string("error1 "));
}
std::string maperror(std::string s) {
    return s + "maperror ";
}

TEST(expected_extensions, issue30) {
    error().map_error(maperror);
}

struct i31 {
    int i;
};
TEST(expected_extensions, issue31) {
    const eu::expected<i31, int> a = i31{42};
    a->i;

    eu::expected<void, std::string> result;
    eu::expected<void, std::string> result2 = result;
    result2 = result;
}

TEST(expected_extensions, issue33) {
    eu::expected<void, int> res{eu::unexpect, 0};
    EXPECT_TRUE(!res);
    res = res.map_error([](int) {
        return 42;
    });
    EXPECT_TRUE(res.error() == 42);
}


eu::expected<void, std::string> voidWork() {
    return {};
}
eu::expected<int, std::string> work2() {
    return 42;
}
void errorhandling(std::string) {}

TEST(expected_extensions, issue34) {
    eu::expected<int, std::string> result = voidWork().and_then(work2);
    result.map_error([&](std::string result) {
        errorhandling(result);
    });
}

struct non_copyable {
    non_copyable(non_copyable&&) = default;
    non_copyable(non_copyable const&) = delete;
    non_copyable() = default;
};

TEST(expected_extensions, issue42) {
    eu::expected<non_copyable, int>{}.map([](non_copyable) {
    });
}

TEST(expected_extensions, issue43) {
    auto result = eu::expected<void, std::string>{};
    result = eu::make_unexpected(std::string{"foo"});
}

#if !(__GNUC__ <= 5)
    #include <memory>

using MaybeDataPtr = eu::expected<int, std::unique_ptr<int>>;

MaybeDataPtr test(int i) noexcept {
    return std::move(i);
}

MaybeDataPtr test2(int i) noexcept {
    return std::move(i);
}

TEST(expected_extensions, issue49) {
    auto m = test(10).and_then(test2);
}
#endif // !(__GNUC__ <= 5)

eu::expected<int, std::unique_ptr<std::string>> func() {
    return 1;
}

TEST(expected_extensions, issue61) {
    EXPECT_TRUE(func().value() == 1);
}
