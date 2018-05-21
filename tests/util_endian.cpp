#include <gtest/gtest.h>
#include <obi/util/endian.hpp>

#include <array>

using namespace ::obi::util;

#ifdef OBI_LITTLE_ENDIAN
TEST(util_endian, is_little){
    ASSERT_TRUE(endian::is_little());
}

TEST(util_endian, host_to_little){
    {
        std::int32_t const x = 42;
        std::int32_t y = endian::host_to_little(x);
        ASSERT_EQ(y,x);
    }

    {
        std::uint32_t const x = 42;
        std::uint32_t y = endian::host_to_little(x);
        ASSERT_EQ(y,x);
    }

    {
        std::int64_t const x = 42;
        std::int64_t y = endian::host_to_little(x);
        ASSERT_EQ(y,x);
    }

    {
        std::uint64_t const x = 42;
        std::uint64_t y = endian::host_to_little(x);
        ASSERT_EQ(y,x);
    }
}

TEST(util_endian, little_to_host){
    {
        std::int32_t const x = 42;
        std::int32_t y = endian::little_to_host(x);
        ASSERT_EQ(y,x);
    }

    {
        std::uint32_t const x = 42;
        std::uint32_t y = endian::little_to_host(x);
        ASSERT_EQ(y,x);
    }

    {
        std::int64_t const x = 42;
        std::int64_t y = endian::little_to_host(x);
        ASSERT_EQ(y,x);
    }

    {
        std::uint64_t const x = 42;
        std::uint64_t y = endian::little_to_host(x);
        ASSERT_EQ(y,x);
    }
}

TEST(util_endian, host_to_big){
    {
        std::uint32_t const x = 0x01020304;
        std::uint32_t y = endian::host_to_big(x);
        ASSERT_EQ(y,0x04030201);
    }
}

TEST(util_endian, big_to_host){
    {
        std::uint32_t const x = 0x01020304;
        std::uint32_t y = endian::big_to_host(x);
        ASSERT_EQ(y,0x04030201);
    }
}

#elif OBI_BIG_ENDIAN
TEST(util_endian, is_little){
    ASSERT_FALSE(endian::is_little());
}
#else
    "fail to compile"
    #pragma message "fail to compile"
#endif
