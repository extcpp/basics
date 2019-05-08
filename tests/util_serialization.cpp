#include <gtest/gtest.h>
#include <obi/util/serialization.hpp>
#include <obi/util/encode.hpp>

#include <array>
#include <vector>
#include <algorithm>

using namespace ::obi::util;

namespace {
    std::uint32_t num = 0x01020304U;
    std::uint32_t num_reverse = 0x04030201U;
    std::uint32_t little_value = 16909060;
    std::uint32_t big_value = 67305985;
}

TEST(util_serialization, assert_assumptions){
    if(endian::is_little()){
        ASSERT_EQ(num,little_value);
        ASSERT_EQ(num_reverse,big_value);
    } else {
        ASSERT_EQ(num,big_value);
        ASSERT_EQ(num_reverse,little_value);
    }
}


TEST(util_serialization, little_storage_behaves_as_pushback){
    std::array<std::byte,sizeof(num)> arr;
    to_little_storage(&arr[0], num); // pointer

    std::string str;
    to_little_storage(str, num); // string

    {   // not cursor advancing method
        ASSERT_EQ(sizeof(num), str.size());
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
    }

    {   // cursor advancing method
        std::byte* cursor = &arr[0];
        to_little_storage_advance(cursor, num);
        auto rv = std::memcmp(&arr[0], str.data(), sizeof(num));
        ASSERT_EQ(rv,0);
        ASSERT_EQ(cursor,&arr[0]+sizeof(num));
    }

    {   // not cursor advancing method
        std::uint32_t num_out = 0;
        from_little_storage(reinterpret_cast<std::byte*>(str.data()), num_out);
        ASSERT_EQ(num, num_out);
    }

    {   // cursor advancing method
        std::byte const* cursor = reinterpret_cast<std::byte*>(str.data());
        std::uint32_t num_out;
        from_little_storage_advance(cursor,num_out);
        ASSERT_EQ(num, num_out);
        ASSERT_EQ(cursor ,reinterpret_cast<std::byte*>(str.data()) + sizeof(std::uint32_t));
    }
}

TEST(util_serialization, little_storage_integral){
    std::vector<std::byte> storage;
    storage.resize(sizeof(num));
    to_little_storage(storage.data(), num);
    if(endian::is_little()){
        ASSERT_TRUE(std::memcmp(&num,storage.data(),sizeof(num)) == 0);
    } else {
        ASSERT_TRUE(std::memcmp(&num_reverse,storage.data(),sizeof(num)) == 0);
    }
}

TEST(util_serialization, big_storage_integral){
    std::vector<std::byte> storage;
    storage.resize(sizeof(num));
    to_big_storage(storage.data(), num);
    if(endian::is_little()){
        ASSERT_TRUE(std::memcmp(&num_reverse,storage.data(),sizeof(num)) == 0);
    } else {
        ASSERT_TRUE(std::memcmp(&num,storage.data(),sizeof(num)) == 0);
    }
}

TEST(util_serialization, little_storage_multi_in_out){
    std::uint64_t a_in = 42;
    std::uint32_t b_in = 23;
    std::vector<std::byte> storage;
    storage.resize(size_of(a_in, b_in));
    to_little_storage(storage.data(),a_in, b_in);

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        std::byte const* cursor = storage.data();
        from_little_storage_advance(cursor, a_out);
        from_little_storage_advance(cursor, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        from_little_storage(storage.data(), a_out, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }
}

TEST(util_serialization, big_storage_multi_in_out){
    std::uint64_t a_in = 42;
    std::uint32_t b_in = 23;
    std::vector<std::byte> storage;
    storage.resize(size_of(a_in, b_in));
    to_big_storage(storage.data(),a_in, b_in);


    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        std::byte const* cursor = storage.data();
        from_big_storage_advance(cursor, a_out);
        from_big_storage_advance(cursor, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        from_big_storage(storage.data(), a_out, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }
}

TEST(util_serialization, little_storage_array_multi_in_out){
    std::uint64_t a_in = 42;
    std::uint32_t b_in = 23;
    auto array = to_little_storage_array(a_in, b_in);
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

    // convenience functions when converting to string
    auto str = std::string(to_char_ptr(array), array.size());
    ASSERT_TRUE(std::memcmp(array.data(),str.data(),array.size()) == 0);
}

TEST(util_serialization, big_storage_array_multi_in_out){
    std::uint64_t a_in = 42;
    std::uint32_t b_in = 23;
    auto array = to_big_storage_array(a_in, b_in);
    ASSERT_EQ(array.size(), size_of(a_in, b_in));

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        std::byte const* cursor = array.data();
        from_big_storage_advance(cursor, a_out);
        from_big_storage_advance(cursor, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }

    {
        std::uint64_t a_out = 0;
        std::uint32_t b_out = 0;
        from_big_storage(array.data(), a_out, b_out);
        ASSERT_EQ(a_in, a_out);
        ASSERT_EQ(b_in, b_out);
    }

    // convenience functions when converting to string
    auto str = std::string(to_char_ptr(array), array.size());
    ASSERT_TRUE(std::memcmp(array.data(),str.data(),array.size()) == 0);
}
