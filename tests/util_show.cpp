#include <ext/util/show.hpp>
#include <gtest/gtest.h>
using namespace std::literals::string_literals;

namespace eu = ::ext::util;

TEST(util_show, string) {
    ASSERT_EQ(eu::show("foo"s), "\"foo\"");
}

TEST(util_show, string_view) {
    auto out = "bar"s;
    ASSERT_EQ(eu::show(std::string_view(out)), "\"bar\"");
}

TEST(util_show, vec_empty) {
    ASSERT_EQ(eu::show(std::vector<int>{}), "[ ]");
}

TEST(util_show, vec_one_item) {
    ASSERT_EQ(eu::show(std::vector{1}), "[1]");
}

TEST(util_show, vec_int) {
    ASSERT_EQ(eu::show(std::vector{1, 2, 3, 4, 5, 6}), "[1, 2, 3, 4, 5, 6]");
}

TEST(util_show, vec_string) {
    ASSERT_EQ(eu::show(std::vector<std::string>{"foo", "bar"}), "[\"foo\", \"bar\"]");
}

TEST(util_show, pair) {
    ASSERT_EQ(eu::show(std::pair{"foo"s, 23}), "(\"foo\", 23)");
}

TEST(util_show, map_int_str) {
    using Map = std::map<int, std::string>;
    Map out{{2, "foo"s}, {3, "bar"s}};
    ASSERT_EQ(eu::show(out), "{2:\"foo\", 3:\"bar\"}");
}

TEST(util_show, vec_map_vec_int_str) {
    using Map = std::map<std::vector<int>, std::string>;
    Map item{{std::vector{1,2}, "foo"s}, {std::vector{3}, "bar"s}};
    std::vector<Map> out = {item, item};
    ASSERT_EQ(eu::show(out), "[{[1, 2]:\"foo\", [3]:\"bar\"}, {[1, 2]:\"foo\", [3]:\"bar\"}]");
}

TEST(util_show, set_int) {
    ASSERT_EQ(eu::show(std::set{1, 2, 3}), "{1, 2, 3}");
}

TEST(util_show, tuple) {
    ASSERT_EQ(eu::show(std::tuple<>{}), "( )");
    ASSERT_EQ(eu::show(std::tuple<int, int, int, std::string>{3, 2 , 1, "risiko"}), "(3, 2, 1, \"risiko\")");
    ASSERT_EQ(eu::show(std::tuple<std::string>{"DarkwingDuck"}), "(\"DarkwingDuck\")");
}

TEST(util_show, vec_bool) {
    ASSERT_EQ(eu::show(std::vector<bool>{true, false}), "[true, false]");
}

// tuple
