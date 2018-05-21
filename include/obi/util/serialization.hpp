#pragma once

#include <obi/util/endian.hpp>

namespace obi { namespace util {

//// to little storage
// with string
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>>
to_little_storage(T in, std::string& out) {
    in = endian::host_to_little(in);
    std::size_t len = 0;
    do {
        out.push_back(static_cast<char>(in & 0xffU));
        in >>= 8;
    } while (++len < sizeof(T));
}

// with pointer - does not advance pointer
// returns advanced pointer position
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>, char*>
to_little_storage(T in, char* out) {
    in = endian::host_to_little(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template <typename T> //constexpr
    std::enable_if_t<std::is_integral_v<T>, void>
    to_little_storage_advance(T in, char*& out) {
        in = endian::host_to_little(in);
        std::memcpy(out, &in, sizeof(std::decay_t<T>));
        out += sizeof(std::decay_t<T>);
}


// writes multiple arguments into an array
template <typename ...T, std::size_t N = size_of<T...>()> //constexpr
std::array<std::byte, N>
to_little_storage_array(T& ...args){
    std::array<std::byte,N> result;
    char* cursor = &result;
    (to_little_storage_advance(args,cursor), ...);
    return result;
}

//// to big storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>, char*>
to_big_storage(T in, char* out) {
    in = endian::host_to_big(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

//// from little storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,char const*>
from_little_storage(char const* in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::little_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
from_little_storage_advance(char const*& in){
    std::decay_t<T> out;
    in = from_little_storage(in,out);
    return out;
}

//// from big storage
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,char const*>
from_big_storage(char const*& in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::big_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
from_big_storage_advance(char*& in){
    std::decay_t<T> out;
    in = from_big_storage(in,out);
    return out;
}

}} // namespace obi::util
