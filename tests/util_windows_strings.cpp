// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <gtest/gtest.h>

#include <string>
using namespace std::literals::string_literals;

#include <ext/util/string.hpp>
#include <ext/util/windows_strings.hpp>

#ifdef EXT_WINDOWS
using namespace ext::util;
TEST(util_windows_strings, error_to_string) {
    ASSERT_EQ(""s, win_error_to_string(0));

    ASSERT_EQ(ERROR_INVALID_SID, 1337);
    ASSERT_EQ("The security ID structure is invalid."s, win_error_to_string(ERROR_INVALID_SID));
}

TEST(util_windows_strings, from_to_ucs16) {
    auto test_string = "der hund"s;
    auto test_wstring = L"der hund"s;

    ASSERT_EQ(test_string, string_from_win(test_wstring));
    ASSERT_EQ(test_string, string_from_win(string_to_win(test_string)));

    ASSERT_EQ(test_wstring, string_to_win(test_string));
    ASSERT_EQ(test_wstring, string_to_win(string_from_win(test_wstring)));
}

TEST(util_windows_strings, empty) {
    ASSERT_EQ(""s, string_from_win(string_to_win(""s)));
}

TEST(util_windows_strings, japanese) {
    auto test_string_jap =
        u8"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをんゔ"s;
    ASSERT_EQ(from_u8string(test_string_jap), string_from_win(string_to_win(test_string_jap)));
}

TEST(util_windows_strings, euro_sign) {
    wchar_t euro = 0x20AC; // https://unicodemap.org/details/0x20AC/index.html
    auto euro_wstring = std::wstring(&euro, 1);
    ASSERT_EQ("€", string_from_win(euro_wstring));
}
#endif // EXT_WINDOWS
