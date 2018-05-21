#include <gtest/gtest.h>
#include <obi/util/serialization.hpp>
#include <obi/util/encode.hpp>

#include <array>

using namespace ::obi::util;

#ifdef OBI_LITTLE_ENDIAN

TEST(util_serialization, little_storage){
    std::uint32_t num = 0x01020304U;

    std::array<char,sizeof(num)> arr;
    to_little_storage(num, &arr[0]); // pointer

    std::string str;
    to_little_storage(num, str); // string

    {   // not cursor advancing method
        ASSERT_EQ(sizeof(num), str.size());
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
    }

    {   // cursor advancing method
        char* cursor = &arr[0];
        to_little_storage_advance(num, cursor);
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
        ASSERT_EQ(cursor,&arr[0]+sizeof(num));
    }

    {   // not cursor advancing method
        std::uint32_t num_out = 0;
        from_little_storage(str.data(), num_out);
        ASSERT_EQ(num, num_out);
    }

    {   // cursor advancing method
        char const* cursor = str.data();
        std::uint32_t num_out = from_little_storage_advance<std::uint32_t>(cursor);
        ASSERT_EQ(num, num_out);
        ASSERT_EQ(cursor ,str.data()+sizeof(std::uint32_t));
    }
}


#elif OBI_BIG_ENDIAN
#else
    "fail to compile"
    #pragma message "fail to compile"
#endif
