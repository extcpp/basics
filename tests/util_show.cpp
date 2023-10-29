// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/pretty.hpp>
#include <gtest/gtest.h>
#include <sstream>

using namespace std::literals::string_literals;

namespace eu = ::ext::util::pretty;

TEST(util_pretty, string) {
    ASSERT_EQ(eu::fmt("foo"s), "foo");
}

TEST(util_pretty, string_view) {
    auto out = "bar"s;
    ASSERT_EQ(eu::fmt(std::string_view(out)), "bar");
}

TEST(util_pretty, vec_empty) {
    ASSERT_EQ(eu::fmt(std::vector<int>{}), "[ ]");
}

TEST(util_pretty, vec_one_item) {
    ASSERT_EQ(eu::fmt(std::vector{1}), "[1]");
}

TEST(util_pretty, vec_int) {
    ASSERT_EQ(eu::fmt(std::vector{1, 2, 3, 4, 5, 6}), "[1, 2, 3, 4, 5, 6]");
}

TEST(util_pretty, vec_string) {
    ASSERT_EQ(eu::fmt(std::vector<std::string>{"foo", "bar"}), "[\"foo\", \"bar\"]");
}

TEST(util_pretty, pair) {
    ASSERT_EQ(eu::fmt(std::pair{"foo"s, 23}), "(\"foo\", 23)");
}

TEST(util_pretty, map_int_str) {
    using Map = std::map<int, std::string>;
    Map out{{2, "foo"s}, {3, "bar"s}};
    ASSERT_EQ(eu::fmt(out), "{2:\"foo\", 3:\"bar\"}");
}

TEST(util_pretty, vec_map_vec_int_str) {
#ifndef CLANG_ANALYZE
    using Map = std::map<std::vector<int>, std::string>;
    Map item{{std::vector{1, 2}, "foo"s}, {std::vector{3}, "bar"s}};
    std::vector<Map> out = {item, item};
    ASSERT_EQ(eu::fmt(out), "[{[1, 2]:\"foo\", [3]:\"bar\"}, {[1, 2]:\"foo\", [3]:\"bar\"}]");
    std::stringstream ss;
    using eu::operator<<;
    ss << out;
    ASSERT_EQ(ss.str(), "[{[1, 2]:\"foo\", [3]:\"bar\"}, {[1, 2]:\"foo\", [3]:\"bar\"}]");
#endif // CLANG_ANALYZE
}

TEST(util_pretty, set_int) {
    ASSERT_EQ(eu::fmt(std::set{1, 2, 3}), "{1, 2, 3}");
}

TEST(util_pretty, tuple) {
    ASSERT_EQ(eu::fmt(std::tuple<>{}), "( )");
    ASSERT_EQ(eu::fmt(std::tuple<int, int, int, std::string>{3, 2, 1, "risiko"}), "(3, 2, 1, \"risiko\")");
    ASSERT_EQ(eu::fmt(std::tuple<std::string>{"DarkwingDuck"}), "(\"DarkwingDuck\")");
    std::stringstream ss;
    using eu::operator<<;
    ss << std::tuple<int, int, int, std::string>{3, 2, 1, "risiko"};
    ASSERT_EQ(ss.str(), "(3, 2, 1, \"risiko\")");
}

TEST(util_pretty, vec_bool) {
    // vector<bool> - seriously? ==> https://github.com/vector-of-bool/dds
    using namespace std;
    ASSERT_EQ(eu::fmt(vector<bool>{true, true, false}), "[true, true, false]");
}
