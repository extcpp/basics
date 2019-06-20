#include <gtest/gtest.h>
#include <ext/util/endian.hpp>

#include <array>

using namespace ::ext::util;

namespace {
    std::uint32_t num = 0x01020304U;
    std::uint32_t num_reverse = 0x04030201U;
    std::uint32_t little_value = 16909060;
    std::uint32_t big_value = 67305985;
}

TEST(util_endian, assert_assumptions){
    if(endian::is_little()){
        ASSERT_EQ(num,little_value);
        ASSERT_EQ(num_reverse,big_value);
    } else {
        ASSERT_EQ(num,big_value);
        ASSERT_EQ(num_reverse,little_value);
    }
}


TEST(util_endian, host_to_little){
    auto x = endian::host_to_little(num);
    if(endian::is_little()){
        ASSERT_EQ(little_value,x);
        ASSERT_EQ(num,x);
    } else {
        ASSERT_EQ(num_reverse,x);
    }

}

TEST(util_endian, little_to_host){
    auto x = endian::little_to_host(num);
    if(endian::is_little()){
        ASSERT_EQ(little_value,x);
        ASSERT_EQ(num,x);
    } else {
        ASSERT_EQ(num_reverse,x);
    }
}

TEST(util_endian, host_to_big){
    auto x = endian::host_to_big(num);
    if(endian::is_little()){
        ASSERT_EQ(big_value,x);
        ASSERT_EQ(num_reverse,x);
    } else {
        ASSERT_EQ(num,x);
    }
}

TEST(util_endian, big_to_host){
    auto x = endian::host_to_big(num_reverse);
    if(endian::is_little()){
        ASSERT_EQ(little_value,x);
        ASSERT_EQ(num,x);
    } else {
        ASSERT_EQ(num_reverse,x);
    }
}
