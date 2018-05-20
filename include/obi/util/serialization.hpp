#pragma once

#include <obi/util/endian.hpp>

namespace obi { namespace util {

// to little storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>>
integral_to_little_storage(T in, std::string& out) {
    in = endian::host_to_little(in);
    std::size_t len = 0;
    do {
        out.push_back(static_cast<char>(in & 0xffU));
        in >>= 8;
    } while (++len < sizeof(T));
}

template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte*>
integral_to_little_storage(T in, std::byte* out) {
    in = endian::host_to_little(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

// to big storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte*>
integral_to_big_storage(T in, std::byte* out) {
    in = endian::host_to_big(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

// from little storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
integral_from_little_storage(std::byte const* in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::little_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
integral_from_little_storage_advance(std::byte const*& in){
    std::decay_t<T> out;
    in = integral_from_little_storage(in,out);
    return out;
}

// from big storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
integral_from_big_storage(std::byte const*& in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::big_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
integral_from_big_storage_advance(std::byte*& in){
    std::decay_t<T> out;
    in = integral_from_big_storage(in,out);
    return out;
}

}} // namespace obi::util
