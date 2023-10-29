// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <cstring>
#include <ext/util/endian.hpp>
#include <ext/util/hex.hpp>
#include <ext/util/string.hpp>
#include <gtest/gtest.h>

using namespace ext::util;
using namespace std::literals;

TEST(util_encode, encode_hex) {
    std::uint32_t in = 0x01020304;
    auto hex = encode_hex(reinterpret_cast<char*>(&in), sizeof(in));

    if (ext::util::endian::is_little()) {
        ASSERT_EQ(hex, "04030201"s);
    } else {
        ASSERT_EQ(hex, "01020304"s);
    }

    auto dehex = decode_hex(hex);
    uint32_t out = 0;
    ASSERT_EQ(sizeof(out), dehex.size());

    std::memcpy(reinterpret_cast<char*>(&out), dehex.data(), sizeof(out));
    ASSERT_EQ(in, out);
}

TEST(util_encode, encode_hex_upper_lower) {
    std::uint32_t in = 0x01aAbBcC;
    auto hex = encode_hex(reinterpret_cast<char*>(&in), sizeof(in));
    auto dehex1 = decode_hex(hex);
    auto dehex2 = decode_hex(to_upper(hex));
    ASSERT_EQ(dehex1, dehex2);
    uint32_t out = 0;
    std::memcpy(reinterpret_cast<char*>(&out), dehex1.data(), sizeof(out));
    ASSERT_EQ(in, out);
}

TEST(util_encode, decode_zero_len) {
    auto dehex = decode_hex(std::string(""));
    ASSERT_EQ(dehex, "");
}

TEST(util_encode, decode_hex_throw) {
    using namespace std::literals::string_literals;
    ASSERT_THROW(decode_hex("UUUUULLLF"s), std::invalid_argument);
}
