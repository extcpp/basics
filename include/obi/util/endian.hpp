#pragma once

#include <cstdint>
#include <cstring>
#include <cstddef>
#include <type_traits>
#include <stdexcept>

#ifdef __APPLE__
    #include <machine/endian.h>
    #include <libkern/OSByteOrder.h>
#elif _WIN32
#elif __linux__
    #include <endian.h>
#else
    #pragma messsage("unsupported os or compiler")
#endif

namespace obi::util::endian {


// evaluated during compile time!!!
constexpr bool is_little() {
    constexpr uint32_t const num = 0x01020304;
    return (static_cast<char const&>(num) == 0x04); // why does std::byte not work
    //              ^ above is the same as:  `*(char const *)&num`
    // integral type with bigger alignment may be casted to cahr, unsigned char, byte
}

namespace {
constexpr void byte_swap(void *ptr, std::size_t bytes) {
    for(std::size_t pos = 0; pos < bytes/2; pos++) {
        auto counter_pos = bytes - pos - 1;
        char swap = static_cast<char*>(ptr)[pos];
        static_cast<char*>(ptr)[pos] = static_cast<char*>(ptr)[counter_pos];
        static_cast<char*>(ptr)[counter_pos] = swap;
    }
}
} // unamed namespace - end

// host to little unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>
host_to_little(T in){
#ifdef __APPLE__
    switch(sizeof(T)) {
        case 2:  { return OSSwapHostToLittleInt16(in); };
        case 4:  { return OSSwapHostToLittleInt32(in); };
        case 8:  { return OSSwapHostToLittleInt64(in); };
        default: { throw  std::logic_error("not implemented") };
    }
#elif __linux__
    switch(sizeof(T)) {
        case 2:  {  using P = decltype(htole16(0));
                    return static_cast<T>(htole16(static_cast<P>(in)));
                 };
        case 4:  {  using P = decltype(htole32(0));
                    return static_cast<T>(htole32(static_cast<P>(in)));
                 };
        case 8:  {  using P = decltype(htole64(0));
                    return static_cast<T>(htole64(static_cast<P>(in)));
                 };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    if(!is_little()) {
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// host to little singed
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>
host_to_little(T in){
    std::make_unsigned_t<std::decay_t<T>> tmp;
    std::memcpy(&tmp, &in, sizeof(T));
    tmp = host_to_little(tmp);
    std::memcpy(&in, &tmp, sizeof(T));
    return in;
}


// little to host unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>
little_to_host(T in){
#ifdef __APPLE__
    switch(sizeof(T)) {
        case 2:  { return OSSwapLittleToHostInt16(in); };
        case 4:  { return OSSwapLittleToHostInt32(in); };
        case 8:  { return OSSwapLittleToHostInt64(in); };
        default: { throw  std::logic_error("not implemented") };
    }
#elif __linux__
    switch(sizeof(T)) {
        case 2:  {  using P = decltype(le16toh(0));
                    return static_cast<T>(le16toh(static_cast<P>(in)));
                 };
        case 4:  {  using P = decltype(le32toh(0));
                    return static_cast<T>(le32toh(static_cast<P>(in)));
                 };
        case 8:  {  using P = decltype(le64toh(0));
                    return static_cast<T>(le64toh(static_cast<P>(in)));
                 };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    if(!is_little()){
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// little to host unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>
little_to_host(T in){
    std::make_unsigned_t<std::decay_t<T>> tmp;
    std::memcpy(&tmp, &in, sizeof(T));
    tmp = little_to_host(tmp);
    std::memcpy(&in, &tmp, sizeof(T));
    return in;
}



// host to big unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>
host_to_big(T in){
#ifdef __APPLE__
    switch(sizeof(T)) {
        case 2:  { return OSSwapHostToBigInt16(in); };
        case 4:  { return OSSwapHostToBigInt32(in); };
        case 8:  { return OSSwapHostToBigInt64(in); };
        default: { throw  std::logic_error("not implemented") };
    }
#elif __linux__
    switch(sizeof(T)) {
        case 2:  {  using P = decltype(htobe16(0));
                    return static_cast<T>(htobe16(static_cast<P>(in)));
                 };
        case 4:  {  using P = decltype(htobe32(0));
                    return static_cast<T>(htobe32(static_cast<P>(in)));
                 };
        case 8:  {  using P = decltype(htobe64(0));
                    return static_cast<T>(htobe64(static_cast<P>(in)));
                 };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    if(is_little()) {
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// host to big singed
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>
host_to_big(T in){
    std::make_unsigned_t<T>  tmp;
    std::memcpy(&tmp, &in, sizeof(T));
    tmp = host_to_big(tmp);
    std::memcpy(&in, &tmp, sizeof(T));
    return in;
}

// big to host unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>
big_to_host(T in){
#ifdef __APPLE__
    switch(sizeof(T)) {
        case 2:  { return OSSwapBigToHostInt16(in); };
        case 4:  { return OSSwapBigToHostInt32(in); };
        case 8:  { return OSSwapBigToHostInt64(in); };
        default: { throw  std::logic_error("not implemented") };
    }
#elif __linux__
    switch(sizeof(T)) {
        case 2:  {  using P = decltype(be16toh(0));
                    return static_cast<T>(be16toh(static_cast<P>(in)));
                 };
        case 4:  {  using P = decltype(be32toh(0));
                    return static_cast<T>(be32toh(static_cast<P>(in)));
                 };
        case 8:  {  using P = decltype(be64toh(0));
                    return static_cast<T>(be64toh(static_cast<P>(in)));
                 };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    if(is_little()) {
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// big to host unsinged
template <typename T> //constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T> ,T>
big_to_host(T in){
    std::make_unsigned_t<std::decay_t<T>> tmp;
    std::memcpy(&tmp, &in, sizeof(T));
    tmp = big_to_host(tmp);
    std::memcpy(&in, &tmp, sizeof(T));
    return in;
}

} // namespace obi::util::endian - end
