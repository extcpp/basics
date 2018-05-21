#include <gtest/gtest.h>
#include <obi/util/serialization.hpp>
#include <obi/util/encode.hpp>

#include <array>

using namespace ::obi::util;

#ifdef OBI_LITTLE_ENDIAN

TEST(util_serialization, little_storage_integrals){
    std::uint32_t num = 0x01020304U;

    std::array<std::byte,sizeof(num)> arr;
    to_little_storage(num, &arr[0]); // pointer

    std::string str;
    to_little_storage(num, str); // string

    {   // not cursor advancing method
        ASSERT_EQ(sizeof(num), str.size());
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
    }

    {   // cursor advancing method
        std::byte* cursor = &arr[0];
        to_little_storage_advance(num, cursor);
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
        ASSERT_EQ(cursor,&arr[0]+sizeof(num));
    }

    {   // not cursor advancing method
        std::uint32_t num_out = 0;
        from_little_storage((std::byte*)str.data(), num_out);
        ASSERT_EQ(num, num_out);
    }

    {   // cursor advancing method
        std::byte const* cursor = (std::byte*)str.data();
        std::uint32_t num_out;
        from_little_storage_advance(cursor,num_out);
        ASSERT_EQ(num, num_out);
        ASSERT_EQ(cursor ,(std::byte*)str.data()+sizeof(std::uint32_t));
    }
}

TEST(util_serialization, little_storage_multi){
    std::uint64_t a_in = 42;
    std::uint32_t b_in = 23;
    auto array = to_little_storage(a_in, b_in);
    ASSERT_EQ(array.size(), size_of(a_in, b_in));


    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        std::byte const* cursor = array.data();
        from_little_storage_advance(cursor, a_out);
        from_little_storage_advance(cursor, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        from_little_storage(array.data(), a_out, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }
}

#elif OBI_BIG_ENDIAN
#else
    "fail to compile"
    #pragma message "fail to compile"
#endif
