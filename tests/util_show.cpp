#include <ext/util/show.hpp>
#include <gtest/gtest.h>

namespace eu = ::ext::util;

TEST(util_show, string) {
    std::string out = "foo";
    ASSERT_EQ(eu::show(out), "\"foo\"");
}

TEST(util_show, string_view) {
    std::string out = "bar";
    ASSERT_EQ(eu::show(out), "\"bar\"");
}

TEST(util_show, vec_empty) {
    std::vector<int> out = { };
    ASSERT_EQ(eu::show(out), "[ ]");
}

TEST(util_show, vec_one_item) {
    std::vector<int> out = {1};
    ASSERT_EQ(eu::show(out), "[1]");
}

TEST(util_show, vec_int) {
    std::vector<int> out = {1, 2, 3, 4, 5, 6};
    ASSERT_EQ(eu::show(out), "[1, 2, 3, 4, 5, 6]");
}

TEST(util_show, vec_string) {
    std::vector<std::string> out = {"foo", "bar"};
    ASSERT_EQ(eu::show(out), "[\"foo\", \"bar\"]");
}

TEST(util_show, pair) {
    std::pair<std::string, int> out = {"foo", 23};
    ASSERT_EQ(eu::show(out), "(\"foo\", 23)");
}

TEST(util_show, map_int_str) {
    using Map = std::map<int, std::string>;
    Map out{{2, "foo"}, {3, "bar"}};
    ASSERT_EQ(eu::show(out), "{2:\"foo\", 3:\"bar\"}");
}

TEST(util_show, vec_map_int_str) {
    using Map = std::map<int, std::string>;
    Map item{{2, "foo"}, {3, "bar"}};
    std::vector<Map> out = {item, item};
    ASSERT_EQ(eu::show(out), "[{2:\"foo\", 3:\"bar\"}, {2:\"foo\", 3:\"bar\"}]");
}

TEST(util_show, set_int) {
    std::set out = {1, 2, 3};
    ASSERT_EQ(eu::show(out), "{1, 2, 3}");
}

TEST(util_show, tuple) {
    std::tuple<> out1;
    ASSERT_EQ(eu::show(out1), "( )");
    std::tuple<int, int, int, std::string> out2 = {3, 2 , 1, "risiko"};
    ASSERT_EQ(eu::show(out2), "(3, 2, 1, \"risiko\")");
    std::tuple<std::string> out3 = {"DarkwingDuck"};
    ASSERT_EQ(eu::show(out3), "(\"DarkwingDuck\")");
}

// tuple
