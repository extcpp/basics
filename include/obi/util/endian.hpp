#pragma once

#include <cstdint>
#include <cstring>
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

namespace obi { namespace util { namespace endian {


// helper
inline bool is_little() {
    int num = 42;
    static bool rv = (*(char*) &num == 42);
    return rv;
}

inline constexpr void byte_swap(void *ptr, std::size_t bytes) {
    for( std::size_t pos = 0; pos < bytes/2; pos++ ) {
        auto couter_pos = bytes - pos - 1;
        uint8_t swap = ((uint8_t*) ptr)[pos];
        ((uint8_t*)ptr)[pos] = ((uint8_t*) ptr)[couter_pos];
        ((uint8_t*)ptr)[couter_pos] = swap;
    }
}

// host to little unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,T>
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
        case 2:  { return htole16(in); };
        case 4:  { return htole32(in); };
        case 8:  { return htole64(in); };
        default: { throw  std::logic_error("not implemented"); };
    }
#ifdef _WIN32
    static bool is_little_endian = is_little();
    if(!is_little_endian){
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// host to little singed
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,T>
host_to_little(T in){
    std::make_unsigned_t<T>  tmp;
    std::memcpy(&tmp,&in,sizeof(T));
    host_to_little(tmp);
    std::memcpy(&in,&tmp,sizeof(T));
return in;
}


// little to host unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,T>
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
        case 2:  { return letoh16(in); };
        case 4:  { return letoh32(in); };
        case 8:  { return letoh64(in); };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    static bool is_little_endian = is_little();
    if(!is_little_endian){
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// little to host unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,T>
little_to_host(T in){
    std::make_unsigned_t<T>  tmp;
    std::memcpy(&tmp,&in,sizeof(T));
    little_to_host(tmp);
    std::memcpy(&in,&tmp,sizeof(T));
return in;
}



// host to big unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,T>
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
        case 2:  { return htobe16(in); };
        case 4:  { return htobe32(in); };
        case 8:  { return htobe64(in); };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    static bool is_little_endian = is_little();
    if(is_little_endian){
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// host to big singed
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,T>
host_to_big(T in){
    std::make_unsigned_t<T>  tmp;
    std::memcpy(&tmp,&in,sizeof(T));
    host_to_big(tmp);
    std::memcpy(&in,&tmp,sizeof(T));
return in;
}


// big to host unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,T>
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
        case 2:  { return betoh16(in); };
        case 4:  { return betoh32(in); };
        case 8:  { return betoh64(in); };
        default: { throw  std::logic_error("not implemented"); };
    }
#elif _WIN32
    static bool is_little_endian = is_little();
    if(is_little_endian){
        byte_swap(&in, sizeof(T));
    }
#endif
    return in;
}

// big to host unsinged
template <typename T>
#ifndef _WIN32
constexpr
#endif
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,T>
big_to_host(T in){
    std::make_unsigned_t<T>  tmp;
    std::memcpy(&tmp,&in,sizeof(T));
    big_to_host(tmp);
    std::memcpy(&in,&tmp,sizeof(T));
return in;
}

}}}
