#include <gtest/gtest.h>
#include <obi/util/encode.hpp>
#include <cstring>

using namespace obi::util;
using namespace std::literals;

TEST(util_encode, encode_hex){
    std::uint32_t in = 0x01020304;
    auto hex = encode_hex(reinterpret_cast<char*>(&in), sizeof(in));
#ifdef OBI_LITTLE_ENDIAN
    ASSERT_EQ(hex, "04030201"s);
#endif
    auto dehex = decode_hex(hex);
    uint32_t out = 0;
    ASSERT_EQ(sizeof(out), dehex.size());
    std::memcpy(reinterpret_cast<char*>(&out), dehex.data(), sizeof(out));
    ASSERT_EQ(in, out);
}

