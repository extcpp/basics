#include <ext/util/cast.hpp>
#include <gtest/gtest.h>

namespace eu = ::ext::util;

TEST(util_cast, size_of) {
    std::uint64_t a = 1;
    std::uint32_t b = 2;
    std::uint16_t c = 2;
    std::uint16_t& cr = c;
    std::uint64_t& ar = a;

    { // two
        std::size_t ext = eu::size_of<std::uint64_t, std::uint32_t>();
        std::size_t norm = sizeof(std::uint64_t) + sizeof(std::uint32_t);
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 12);
    }

    { // many
        std::size_t ext = eu::size_of(a, b, c);
        std::size_t norm = sizeof(a) + sizeof(b) + sizeof(c);
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 14);
    }

    { // reference
        std::size_t ext = eu::size_of(ar);
        std::size_t norm = sizeof(ar);
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 8);
    }

    { // reference
        std::size_t ext = eu::size_of(cr);
        std::size_t norm = sizeof(cr);
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 2);
    }

    { // string
        std::string foo = "Agathe Bauer";
        std::size_t ext = eu::size_of(foo);
        std::size_t norm = sizeof(foo.front()) * foo.size();
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 12);
    }

    { // array
        std::array<std::uint64_t, 3> foo;
        std::size_t ext = eu::size_of(foo);
        std::size_t norm = sizeof(foo.front()) * foo.size();
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 24);
    }

    { // carray
        std::uint32_t foo[3];
        std::size_t ext = eu::size_of(foo);
        std::size_t norm = sizeof(foo[0]) * 3;
        ASSERT_EQ(ext, norm);
        ASSERT_EQ(ext, 12);
    }
}

TEST(util_enum, to_un_signed) {
    int32_t value = 5;
    uint32_t uvalue = 5;

    ASSERT_EQ(eu::to_signed(uvalue), value);
    ASSERT_EQ(eu::to_signed_checked(uvalue), value);

    ASSERT_EQ(eu::to_unsigned(value), uvalue);
    ASSERT_EQ(eu::to_unsigned_checked(value), uvalue);

    ASSERT_EQ(value, eu::to_signed(eu::to_unsigned(value)));
    ASSERT_EQ(uvalue, eu::to_unsigned(eu::to_signed(uvalue)));

    ASSERT_THROW(eu::to_unsigned_checked(-1), std::logic_error);
    ASSERT_THROW(eu::to_unsigned_checked(std::numeric_limits<int32_t>::min()), std::logic_error);
    ASSERT_THROW(eu::to_signed_checked(std::numeric_limits<uint32_t>::max()), std::logic_error);
}
