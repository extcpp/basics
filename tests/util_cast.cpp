#include <gtest/gtest.h>
#include <obi/util/cast.hpp>

namespace ou = ::obi::util;

TEST(util_cast, size_of){
    std::uint64_t a = 1;
    std::uint32_t b = 2;
    std::uint16_t c = 2;
    std::uint16_t& cr = c;
    std::uint64_t& ar = a;

    {   // two
        std::size_t obi = ou::size_of<std::uint64_t,std::uint32_t>();
        std::size_t norm = sizeof(std::uint64_t) + sizeof(std::uint32_t);
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 12);
    }

    {   // many
        std::size_t obi = ou::size_of(a,b,c);
        std::size_t norm = sizeof(a) + sizeof(b) + sizeof(c);
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 14);
    }

    {   // reference
        std::size_t obi = ou::size_of(ar);
        std::size_t norm = sizeof(ar);
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 8);
    }

    {   // reference
        std::size_t obi = ou::size_of(cr);
        std::size_t norm = sizeof(cr);
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 2);
    }

    {   // string
        std::string foo = "Agathe Bauer";
        std::size_t obi = ou::size_of(foo);
        std::size_t norm = sizeof(foo.front()) * foo.size();
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 12);
    }

    {   // array
        std::array<std::uint64_t, 3> foo;
        std::size_t obi = ou::size_of(foo);
        std::size_t norm = sizeof(foo.front()) * foo.size();
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 24);
    }

    {   // carray
        std::uint32_t foo[3];
        std::size_t obi = ou::size_of(foo);
        std::size_t norm = sizeof(foo[0]) * 3;
        ASSERT_EQ(obi, norm);
        ASSERT_EQ(obi, 12);
    }
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

TEST(util_cast, to_un_signed){
    int32_t  value = 5;
    uint32_t uvalue = 5;

    ASSERT_EQ(ou::to_signed(uvalue), value);
    ASSERT_EQ(ou::to_signed_checked(uvalue), value);

    ASSERT_EQ(ou::to_unsigned(value), uvalue);
    ASSERT_EQ(ou::to_unsigned_checked(value), uvalue);

    ASSERT_EQ(value, ou::to_signed(ou::to_unsigned(value)));
    ASSERT_EQ(uvalue, ou::to_unsigned(ou::to_signed(uvalue)));

    ASSERT_THROW(ou::to_unsigned_checked(-1), std::logic_error);
    ASSERT_THROW(ou::to_unsigned_checked(std::numeric_limits<int32_t>::min()), std::logic_error);
    ASSERT_THROW(ou::to_signed_checked(std::numeric_limits<uint32_t>::max()), std::logic_error);
}
