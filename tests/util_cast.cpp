#include <gtest/gtest.h>
#include <obi/util/cast.hpp>

namespace ou = ::obi::util;

TEST(util_cast, size_of){
    std::size_t obi = ou::size_of<std::uint64_t,std::uint32_t>();
    std::size_t norm = sizeof(std::uint64_t) + sizeof(std::uint32_t);
    ASSERT_EQ(obi, norm);
    ASSERT_EQ(obi, 12);
}

TEST(util_cast, enum_to_underlying){
    enum class enum_a : int {
        one = 1,
        two = 2,
        three = 3
    };

    ASSERT_EQ(ou::enum_to_underlying_unsafe(enum_a::one), 1);
    ASSERT_EQ(ou::enum_to_underlying<int>(enum_a::one), 1);
}

TEST(util_cast, underlying_to_enum){
    enum class enum_a : int {
        one = 1,
        two = 2,
        three = 3
    };

    enum class enum_b : long {
        one = 1,
        two = 2,
        three = 3
    };

    ASSERT_EQ(ou::underlying_to_enum<enum_a>(2), enum_a::two);
    ASSERT_EQ(ou::underlying_to_enum<enum_b>(2L), enum_b::two);
}

TEST(util_cast, enum_to_enum){
    enum class enum_a : int {
        one = 1,
        two = 2,
        three = 3
    };

    enum class enum_b : int {
        one = 1,
        two = 2,
        three = 3
    };

    enum class enum_c : long {
        one = 1,
        two = 2,
        three = 3
    };

    ASSERT_EQ(ou::enum_to_enum<enum_b>(enum_a::three), enum_b::three);
}
