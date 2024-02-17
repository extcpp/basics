// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_MEMSTREAM_HEADER
#define EXT_UTIL_MEMSTREAM_HEADER
#pragma once

#include <istream>
#include <streambuf>

namespace ext::util {

// read only
template <typename T>
struct membuf : public std::basic_streambuf<std::remove_cvref_t<T>> {
    membuf(T const* c_ptr, std::size_t size) {
        this->reset(c_ptr, size);
    }

    void reset(T const* c_ptr, std::size_t size) {
        T* ptr = const_cast<T*>(c_ptr); // this cast is not 100% correct
        this->setg(ptr, ptr, ptr + size);
    }
};

inline membuf<std::string_view::value_type> make_membuf(std::string_view const& view) {
    return membuf<std::string_view::value_type>(view.data(), view.length());
}

template <typename T>
struct imemstream : std::basic_istream<T> {
    imemstream(T const* p, std::size_t size) : std::istream(&buffer), buffer(p, size) {
        this->rdbuf(&buffer);
    }

    void reset(T const* p, std::size_t size) {
        buffer.reset(p, size);
        this->init(&buffer);
    }

private:
    membuf<T> buffer;
};

struct viewstream : imemstream<std::string_view::value_type> {
    using base_t = imemstream<std::string_view::value_type>;
    viewstream(std::string_view const& view) : base_t(view.data(), view.length()) {}
    viewstream() : base_t(nullptr, 0) {}
    void reset(std::string_view const& view) {
        base_t::reset(view.data(), view.length());
    }
};

} // namespace ext::util
#endif // EXT_UTIL_MEMSTREAM_HEADER
