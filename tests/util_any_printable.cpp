// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/any_printable.hpp>
#include <gtest/gtest.h>

#include <array>

using namespace ::ext::util;

// extra type
struct developer {
    std::string name;
};
inline std::ostream& operator<<(std::ostream& os, developer const& d) {
    return os << d.name;
}

// access functions
std::string to_string(any_printable const& any) {
    return any.str();
}

std::string to_stream(any_printable const& any) {
    std::stringstream ss;
    any.write_to(ss);
    return ss.str();
}

TEST(util_any_printable, broken) {
    auto d = any_printable(developer{"ulf"});
    to_stream(d);
}

TEST(util_any_printable, string) {
    ASSERT_EQ("42", to_string(42));
    ASSERT_EQ("test", to_string("test"));
    ASSERT_EQ("test", to_string(std::string("test")));
    ASSERT_EQ("ulf", to_string(developer{"ulf"}));
}

TEST(util_any_printable, stream) {
    ASSERT_EQ("42", to_stream(42));
    ASSERT_EQ("test", to_stream("test"));
    ASSERT_EQ("test", to_stream(std::string("test")));
    ASSERT_EQ("ulf", to_stream(developer{"ulf"}));
}
