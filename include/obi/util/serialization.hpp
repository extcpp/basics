#pragma once

#include <obi/util/endian.hpp>

namespace obi { namespace util {

char* to_char_ptr(std::byte* in){
    return reinterpret_cast<char*>(in);
}

char const* to_char_ptr(std::byte const* in){
    return reinterpret_cast<char const*>(in);
}

template<std::size_t N>
char* to_char_ptr(std::array<std::byte,N>& arr){
    return reinterpret_cast<char*>(arr.data());
}

template<std::size_t N>
char const* to_char_ptr(std::array<std::byte,N> const& arr){
    return reinterpret_cast<char const*>(arr.data());
}

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
std::enable_if_t<std::is_integral_v<T>, std::byte*>
to_little_storage(T in, std::byte* out) {
    in = endian::host_to_little(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template <typename T> //constexpr
    std::enable_if_t<std::is_integral_v<T>, void>
    to_little_storage_advance(T in, std::byte*& out) {
        in = endian::host_to_little(in);
        std::memcpy(out, &in, sizeof(std::decay_t<T>));
        out += sizeof(std::decay_t<T>);
}

// writes multiple arguments into an array
template <typename ...T, std::size_t N = size_of<T...>()> //constexpr
std::array<std::byte, N>
to_little_storage(T& ...args){
    std::array<std::byte,N> result;
    std::byte* cursor = result.data();
    (to_little_storage_advance(args,cursor), ...);
    return result;
}
//// to little storage - end

//// to big storage
// with pointer - does not advance pointer
// returns advanced pointer position
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte*>
to_big_storage(T in, std::byte* out) {
    in = endian::host_to_big(in);
    std::memcpy(out, &in, sizeof(std::decay_t<T>));
    return out + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template <typename T> //constexpr
    std::enable_if_t<std::is_integral_v<T>, void>
    to_big_storage_advance(T in, std::byte*& out) {
        in = endian::host_to_big(in);
        std::memcpy(out, &in, sizeof(std::decay_t<T>));
        out += sizeof(std::decay_t<T>);
}

// writes multiple arguments into an array
template <typename ...T, std::size_t N = size_of<T...>()> //constexpr
std::array<std::byte, N>
to_big_storage(T& ...args){
    std::array<std::byte,N> result;
    std::byte* cursor = result.data();
    (to_big_storage_advance(args,cursor), ...);
    return result;
}
//// to big storage - end

//// from little storage
// with pointer - does not advance pointer
// returns advanced pointer position
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::byte const*>
from_little_storage(std::byte const* in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::little_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,void>
from_little_storage_advance(std::byte const*& in, T& out){
    in = from_little_storage(in,out);
}

template <typename ...T> //constexpr
void from_little_storage(std::byte const* in, T& ...outs){
    (from_little_storage_advance(in,outs), ...);
}
//// from little storage - end

//// from big storage
// with pointer - does not advance pointer
// returns advanced pointer position
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,std::byte const*>
from_big_storage(std::byte const*& in, T& out){
    std::memcpy(&out, in, sizeof(std::decay_t<T>));
    out = endian::big_to_host(out);
    return in + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T>,void>
from_big_storage_advance(std::byte*& in,T& out){
    in = from_big_storage(in,out);
}

template <typename ...T> //constexpr
void from_big_storage(std::byte const* in, T& ...outs){
    (from_big_storage_advance(in,outs), ...);
}
//// from big storage - end

}} // namespace obi::util
