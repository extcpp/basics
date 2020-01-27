// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_TO_STR_EAM_HEADER
#define EXT_UTIL_TO_STR_EAM_HEADER
#include <ext/util/result.hpp>
#include <filesystem>

// convert types libext types to string or wirte them in streams

namespace ext { namespace util {

inline std::ostream& operator<<(std::ostream& out, ext::util::result const& res) {
    out << std::boolalpha << ", ok:" << res.ok() << ", code:" << res.code << ", message:'" << res.message << "'";
    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, ext::util::typed_result<T> const& res) {
    out << std::boolalpha << ", ok:" << res.ok() << ", code:" << res.code() << ", message:'" << res.message() << "'"
        << ", vaild:" << res.valid;
    return out;
}

inline std::string to_type_string(::std::filesystem::recursive_directory_iterator const& iter) {
    using namespace ::std::literals;
    if (iter->is_symlink()) {
        return "symlink"s;
    }
    if (iter->is_regular_file()) {
        return "file"s;
    }
    if (iter->is_directory()) {
        return "directory"s;
    }
    if (iter->is_block_file()) {
        return "block file"s;
    }
    if (iter->is_character_file()) {
        return "character file"s;
    }
    if (iter->is_fifo()) {
        return "fifo"s;
    }
    if (iter->is_other()) {
        return "other"s;
    }
    return "unknown"s;
}
}}     // namespace ext::util
#endif // EXT_UTIL_TO_STR_EAM_HEADER
