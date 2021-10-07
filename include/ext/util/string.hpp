// Copyright - 2018 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_STRING_HEADER
#define EXT_UTIL_STRING_HEADER
#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>

#ifdef EXT_USE_TERM
    #include <ext/util/term.hpp>
#endif // EXT_USE_TERM

namespace ext { namespace util {

inline std::string from_u8string(const std::string &s) {
  return s;
}

inline std::string from_u8string(std::string &&s) {
  return std::move(s);
}

#if defined(__cpp_lib_char8_t)
inline std::string from_u8string(const std::u8string &s) {
  return std::string(s.begin(), s.end());
}
#endif

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


namespace _detail::split {
inline std::size_t first_char_matches(std::string::value_type const c,
                                      std::vector<std::string_view> const& seperators,
                                      std::vector<std::string_view>& positions,
                                      bool ordered) {

    std::size_t rv = 0;
    positions.clear();

    for (auto const& sep : seperators) {
        if (sep.empty())
            continue;

        if (c == sep.front()) {
            auto const length = sep.size();
            if (!ordered && length == 1) {
                return 1;
            } else {
                rv = std::max(rv, length);
                positions.push_back(sep);
            }
        }
    }
    return rv;
}
} // namespace _detail::split


inline std::vector<std::string_view> split_on_multiple(std::string const& to_split,
                                                       std::vector<std::string> const& seperators = {" "},
                                                       bool ordered = true) {

    std::vector<std::string_view> seps(seperators.begin(), seperators.end());

    if (ordered) {
        std::stable_sort(seps.begin(), seps.end());
    }

    std::vector<std::string_view> rv;

    auto const data = to_split.data();
    auto const begin = to_split.begin();
    auto const end = to_split.end();

    auto start = begin;
    auto current = begin;
    std::vector<std::string_view> candidates;

    auto add_word = [&]() {
        auto const offset = std::distance(begin, start);
        auto const pointer = to_split.data() + offset;
        auto const length = std::distance(start, current);

        std::string_view part(pointer, length);
        rv.push_back(std::move(part));
    };

    while (current != end) {
        candidates.clear();

        // matches first char only and returns all possible candidates
        // as well as the length of the longest match
        auto length = _detail::split::first_char_matches(*current, seps, candidates, ordered);
        if (length == 0) {
            // no match
            current++;
        } else if (length == 1) {
            add_word();
            current++;
            start = current;
        } else {
            auto current_view =
                std::string_view{data + std::distance(begin, current), (std::size_t) std::distance(current, end)};
            bool matched = false;
            for (auto const& candidate : candidates) {
                if (ext::util::starts_with(current_view, candidate)) {
                    matched = true;
                    add_word();

                    current += std::min(candidate.length(), (size_t) std::distance(current, end));
                    start = current;

                    break;
                }
            }

            if (!matched)
                current++;
        }

        assert(current <= end);
    }

    if (start != current) {
        add_word();
    }

    return rv;
}

}}     // namespace ext::util
#endif // EXT_UTIL_STRING_HEADER
