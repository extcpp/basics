// Copyright - 2018 - Jan Christoph Uhde <Jan@UhdeJC.com>
// requires c++17 (string_view)
#ifndef EXT_UTIL_STRING_HEADER
#define EXT_UTIL_STRING_HEADER

#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>

#ifdef EXT_USE_TERM
    #include "term.hpp"
#endif // EXT_USE_TERM

namespace ext { namespace util {

inline std::string section(std::string const& text, std::size_t width = 80, char fill = '=') {
#ifdef EXT_USE_TERM
    auto size = get_term_size();
    if (size.has_value()) {
        width = std::min(width, size.value().first);
    }
#endif // EXT_USE_TERM
    if (text.length() + 2 >= width) {
        return text;
    }

    if (text.empty()) {
        return std::string(width, fill);
    }

    auto to_fill = width - text.length();
    auto odd = to_fill % 2;
    to_fill -= odd;
    auto half = (to_fill / 2) - 1;

    return std::string(half + odd, fill) + " " + text + " " + std::string(half, fill);
}

inline char to_upper(char ch) {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

inline char to_lower(char ch) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

inline std::string to_upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return s;
}

inline std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return s;
}

// c++20 string has a member function starts_with
inline auto starts_with(std::string_view const& str, std::string_view const& prefix) -> bool {
    if (str.size() < prefix.size())
        return false;
    std::string_view str_short(str.data(), prefix.size());
    return str_short == prefix;
}

// c++20 string has a member function ends_with
inline auto ends_with(std::string_view const& str, std::string_view const& suffix) -> bool {
    if (str.size() < suffix.size())
        return false;
    std::size_t suffix_size = suffix.size();
    std::string_view str_short(str.data() + str.size() - suffix_size, suffix_size);
    return str_short == suffix;
}

// StringType should behave like string /  string_view
template<typename StringType>
inline auto split_on(std::string_view const& str, std::string_view const& seq, bool add_empty = false)
    -> std::vector<StringType> {
    auto rv = std::vector<StringType>{};

    if (seq.size()) {
        std::size_t start_pos = 0;
        auto len = str.size();

        while (start_pos < len) {
            std::size_t new_pos = str.find(seq, start_pos);
            if (new_pos == std::string::npos)
                break;
            std::size_t substr_len = new_pos - start_pos;
            if (substr_len || add_empty) {
                rv.emplace_back(str.substr(start_pos, substr_len));
            }
            start_pos = new_pos + seq.size();
        }

        std::size_t substr_len = len - start_pos;
        if (substr_len || add_empty) {
            rv.emplace_back(str.substr(start_pos, substr_len));
        }
    } else {
        rv.emplace_back(str);
    }
    return rv;
}

inline auto replace(std::string_view const& str, std::string_view const& seq, std::string_view const& replacement)
    -> std::string {
    auto rv = std::string{};
    auto tmp = split_on<std::string_view>(str, seq, true);
    rv.reserve(str.size() + (replacement.size() - seq.size()) * tmp.size());
    if (seq.size() && tmp.size() > 1) {
        for (auto it = tmp.begin(); it + 1 != tmp.end(); it++) {
            rv.append(it->begin(), it->end());
            rv.append(replacement.begin(), replacement.end());
        }
    }
    assert(tmp.size()); // tmp must contain at least one element
    rv.append(tmp.back().begin(), tmp.back().end());
    return rv;
}
}}     // namespace ext::util
#endif // EXT_UTIL_STRING_HEADER
