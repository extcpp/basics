#include <ext/util/string.hpp>
#include <gtest/gtest.h>

using namespace ext::util;

TEST(util_string, to_upper_lower) {
    std::string upper = "UUUULLF";
    std::string lower = "uuuullf";
    ASSERT_EQ(to_lower(upper), lower);
    ASSERT_EQ(to_upper(lower), upper);
    ASSERT_EQ(to_lower(upper[0]), lower[0]);
    ASSERT_EQ(to_upper(lower[0]), upper[0]);
}
TEST(util_string, starts_with) {
    ASSERT_TRUE(starts_with("ein schwein", ""));
    ASSERT_TRUE(starts_with("ein schwein", "ein"));
    ASSERT_FALSE(starts_with("ein schwein", "wein"));
    ASSERT_FALSE(starts_with("ein schwein", "ein schwein too long"));
}

TEST(util_string, ends_with) {
    ASSERT_TRUE(ends_with("ein schwein", ""));
    ASSERT_TRUE(ends_with("ein schwein", "schwein"));
    ASSERT_FALSE(ends_with("ein schwein", "bein"));
    ASSERT_FALSE(ends_with("ein schwein", "too long ein schwein"));
}

TEST(util_string, split_on) {
    ASSERT_EQ(split_on<std::string>("foo::bar", "::"), std::vector<std::string>({"foo", "bar"}));
    ASSERT_EQ(split_on<std::string>("foo::bar::", "::"), std::vector<std::string>({"foo", "bar"}));
    ASSERT_EQ(split_on<std::string>("::foo::::bar::", "::"), std::vector<std::string>({"foo", "bar"}));
    ASSERT_EQ(split_on<std::string>("::foo::::bar::", "::", true /* add_empty*/),
              std::vector<std::string>({"", "foo", "", "bar", ""}));
}

TEST(util_string, replace) {
    ASSERT_EQ(replace("/this/is/path", "/", "\\"), std::string("\\this\\is\\path"));
    ASSERT_EQ(replace("/this/is/path/", "/", "\\"), std::string("\\this\\is\\path\\"));
    ASSERT_EQ(replace("this/is/path/", "/", "\\"), std::string("this\\is\\path\\"));
    ASSERT_EQ(replace("Der Hund hat 3 Beine", "3", "4"), std::string("Der Hund hat 4 Beine"));
    ASSERT_EQ(replace("Der Hund hat 3 Beine", "3 Beine", "ne Leine"), std::string("Der Hund hat ne Leine"));
    ASSERT_EQ(replace("Der Hund hat 3 Beine", "", ""), std::string("Der Hund hat 3 Beine"));
    ASSERT_EQ(replace("Der Hund hat 3 Beine", "", "Leine"), std::string("Der Hund hat 3 Beine"));
    ASSERT_EQ(replace("Der Hund hat 3 Beine", "Z", ""), std::string("Der Hund hat 3 Beine"));
    ASSERT_EQ(replace("", "", ""), std::string(""));
    ASSERT_EQ(replace("", "", "y"), std::string(""));
    ASSERT_EQ(replace("", "x", "y"), std::string(""));
}
