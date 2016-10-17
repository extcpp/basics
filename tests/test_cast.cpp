#include <gtest/gtest.h>
#include <obi/util/cast.hpp>

TEST(util_cast, enum_to_underlying){
    enum class enum_a{
        one = 1,
        two = 2,
        three = 3
    };
    ASSERT_EQ(obi::util::enum_to_underlying(enum_a::one), 1);
}

TEST(util_cast, underlying_to_enum){
    enum class enum_a{
        one = 1,
        two = 2,
        three = 3
    };
    ASSERT_EQ(obi::util::underlying_to_enum<enum_a>(2), enum_a::two);
}

TEST(util_cast, enum_to_enum){
    enum class enum_a{
        one = 1,
        two = 2,
        three = 3
    };
    enum class enum_b{
        one = 1,
        two = 2,
        three = 3
    };
    ASSERT_EQ(obi::util::enum_to_enum<enum_b>(enum_a::three), enum_b::three);
}
