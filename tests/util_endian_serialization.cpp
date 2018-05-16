#include <gtest/gtest.h>
#include <obi/util/serialization.hpp>

using namespace ::obi::util;

TEST(util_endian, is_little){
    ASSERT_TRUE(endian::is_little());
}

//TODO - cmake support
#ifdef OBI_LITTLE_ENDIAN
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
        std::int32_t const x = 0x01020304;
        std::int32_t y = endian::host_to_big(x);
        ASSERT_EQ(y,0x04030201);
    }

//    {
//        std::uint32_t const x = 42;
//        std::uint32_t y = endian::host_to_big(x);
//        ASSERT_EQ(y,x);
//    }
//
//    {
//        std::int64_t const x = 42;
//        std::int64_t y = endian::host_to_big(x);
//        ASSERT_EQ(y,x);
//    }
//
//    {
//        std::uint64_t const x = 42;
//        std::uint64_t y = endian::host_to_big(x);
//        ASSERT_EQ(y,x);
//    }
}

TEST(util_endian, big_to_host){
//    {
//        std::int32_t const x = 42;
//        std::int32_t y = endian::big_to_host(x);
//        ASSERT_EQ(y,x);
//    }
//
//    {
//        std::uint32_t const x = 42;
//        std::uint32_t y = endian::big_to_host(x);
//        ASSERT_EQ(y,x);
//    }
//
//    {
//        std::int64_t const x = 42;
//        std::int64_t y = endian::big_to_host(x);
//        ASSERT_EQ(y,x);
//    }
//
//    {
//        std::uint64_t const x = 42;
//        std::uint64_t y = endian::big_to_host(x);
//        ASSERT_EQ(y,x);
//    }
}


#elif OBI_BIG_ENDIAN
#else
    fail
#endif
