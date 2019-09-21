// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_SERIALIZATION_HEADER
#define EXT_UTIL_SERIALIZATION_HEADER

#include <ext/util/endian.hpp>

namespace ext { namespace util {

char* to_char_ptr(std::byte* in) {
    return reinterpret_cast<char*>(in);
}

char const* to_char_ptr(std::byte const* in) {
    return reinterpret_cast<char const*>(in);
}

template<std::size_t N>
char* to_char_ptr(std::array<std::byte, N>& arr) {
    return reinterpret_cast<char*>(arr.data());
}

template<std::size_t N>
char const* to_char_ptr(std::array<std::byte, N> const& arr) {
    return reinterpret_cast<char const*>(arr.data());
}

//// to little storage
// with string
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>> to_little_storage(std::string& storage, T in) {
    in = endian::host_to_little(in);
    std::size_t len = 0;
    do {
        storage.push_back(static_cast<char>(in & 0xffU));
        in >>= 8;
    } while (++len < sizeof(T));
}

// with pointer - does not advance pointer
// returns advanced pointer position
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte*> to_little_storage(std::byte* storage, T in) {
    in = endian::host_to_little(in);
    std::memcpy(storage, &in, sizeof(std::decay_t<T>));
    return storage + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, void> to_little_storage_advance(std::byte*& storage, T in) {
    in = endian::host_to_little(in);
    std::memcpy(storage, &in, sizeof(std::decay_t<T>));
    storage += sizeof(std::decay_t<T>);
}

template<typename... T>
void to_little_storage_advance(std::byte*& storage, T&... in_args) {
    (to_little_storage_advance(storage, in_args), ...);
}

template<typename... T>
std::byte* to_little_storage(std::byte* storage, T&... in_args) {
    std::byte* cursor = storage;
    (to_little_storage_advance(cursor, in_args), ...);
    return cursor;
}

// writes multiple arguments into an array
template<typename... T, std::size_t N = size_of<T...>()> // constexpr
std::array<std::byte, N> to_little_storage_array(T&... in_args) {
    std::array<std::byte, N> result;
    std::byte* cursor = result.data();
    (to_little_storage_advance(cursor, in_args), ...);
    return result;
}
//// to little storage - end

//// to big storage
// with pointer - does not advance pointer
// returns advanced pointer position
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte*> to_big_storage(std::byte* storage, T in) {
    in = endian::host_to_big(in);
    std::memcpy(storage, &in, sizeof(std::decay_t<T>));
    return storage + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, void> to_big_storage_advance(std::byte*& storage, T in) {
    in = endian::host_to_big(in);
    std::memcpy(storage, &in, sizeof(std::decay_t<T>));
    storage += sizeof(std::decay_t<T>);
}

template<typename... T>
void to_big_storage_advance(std::byte*& storage, T&... in_args) {
    (to_big_storage_advance(storage, in_args), ...);
}

template<typename... T>
std::byte* to_big_storage(std::byte* storage, T&... in_args) {
    std::byte* cursor = storage;
    (to_big_storage_advance(cursor, in_args), ...);
    return cursor;
}

// writes multiple arguments into an array
template<typename... T, std::size_t N = size_of<T...>()> // constexpr
std::array<std::byte, N> to_big_storage_array(T&... in_args) {
    std::array<std::byte, N> result;
    std::byte* cursor = result.data();
    (to_big_storage_advance(cursor, in_args), ...);
    return result;
}
//// to big storage - end

//// from little storage
// with pointer - does not advance pointer
// returns advanced pointer position
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte const*> from_little_storage(std::byte const* storage, T& out) {
    std::memcpy(&out, storage, sizeof(std::decay_t<T>));
    out = endian::little_to_host(out);
    return storage + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, void> from_little_storage_advance(std::byte const*& storage, T& out) {
    storage = from_little_storage(storage, out);
}

template<typename... T> // constexpr
std::byte const* from_little_storage(std::byte const* storage, T&... outs) {
    (from_little_storage_advance(storage, outs), ...);
    return storage;
}
//// from little storage - end

//// from big storage
// with pointer - does not advance pointer
// returns advanced pointer position
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, std::byte const*> from_big_storage(std::byte const*& storage, T& out) {
    std::memcpy(&out, storage, sizeof(std::decay_t<T>));
    out = endian::big_to_host(out);
    return storage + sizeof(std::decay_t<T>);
}

// with pointer ref - advances cursor
template<typename T> // constexpr
std::enable_if_t<std::is_integral_v<T>, void> from_big_storage_advance(std::byte const*& storage, T& out) {
    storage = from_big_storage(storage, out);
}

template<typename... T> // constexpr
std::byte const* from_big_storage(std::byte const* storage, T&... outs) {
    (from_big_storage_advance(storage, outs), ...);
    return storage;
}
//// from big storage - end
}}     // namespace ext::util
#endif // EXT_UTIL_SERIALIZATION_HEADER
