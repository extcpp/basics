#include <gtest/gtest.h>
#include <obi/util/ascii_string_helper.hpp>

using namespace obi::util;

TEST(ascii_string_helper, starts_with){
    ASSERT_TRUE(starts_with("ein schwein", ""));
    ASSERT_TRUE(starts_with("ein schwein", "ein"));
    ASSERT_FALSE(starts_with("ein schwein", "wein"));
}

TEST(ascii_string_helper, ends_with){
    ASSERT_TRUE(ends_with("ein schwein", ""));
    ASSERT_TRUE(ends_with("ein schwein", "schwein"));
    ASSERT_FALSE(ends_with("ein schwein", "bein"));
}

TEST(ascii_string_helper, split_on){
    ASSERT_EQ(split_on("foo::bar", "::")
                ,std::vector<std::string>({"foo","bar"})
                );
    ASSERT_EQ(split_on("foo::bar::", "::")
                ,std::vector<std::string>({"foo","bar"})
                );
    ASSERT_EQ(split_on("::foo::::bar::", "::")
                ,std::vector<std::string>({"foo","bar"})
                );
    ASSERT_EQ(split_on("::foo::::bar::", "::", true /* add_empty*/)
                ,std::vector<std::string>({"","foo","","bar",""})
                );
}
