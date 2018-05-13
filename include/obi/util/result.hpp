#pragma once
#ifndef OBI_UTIL_RESULT_HPP
#define OBI_UTIL_RESULT_HPP

#include <type_traits>
#include <memory>
#include <cstdint>
#include <string>
#include <iostream>

namespace obi { namespace util {

inline int OBI_OK = 0;
inline int OBI_FAIL = 1;
inline int OBI_ERROR_NET = 10;

}} // obi::util

// REMOVE ///////////////////////////////////////////////////////////
#define OBI_RESULT_NOT_FINISHED
#ifdef OBI_RESULT_NOT_FINISHED
#include <map>
// should be in cpp
inline std::string error_code_vo_string(int code) {
  static const std::map<int,std::string> error_map = {
      { obi::util::OBI_ERROR_NET, "network error" }
  };

  auto found = error_map.find(code);
  if(found != error_map.end()) {
      return found->second;
  } else {
      return std::string();
  }
};
#endif
// REMOVE - END /////////////////////////////////////////////////////

namespace obi { namespace util {

struct result {
    result() noexcept
        :code(OBI_OK)
    {}

    result(int num) noexcept
        :code(num)
    {}

    result(int num, std::string const& msg)
        :code(num)
        ,message(msg)
    {}

    result(int num, std::string&& msg)
        :code(num)
        ,message(std::move(msg))
    {}

    result(result const& other)
        :code(other.code)
        ,message(other.message)
    {}

    result(result&& other) noexcept
        :code(other.code)
        ,message(std::move(other.message))
    {}

    auto operator=(result const& other)
    -> result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    auto operator=(result&& other) noexcept
    -> result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }

    auto get_message() const
    -> std::string {
        if (message.empty()) {
            if(code != OBI_OK) {
                message = error_code_vo_string(code);
            }
        }
        return message;
    }

    auto get_code() const -> int { return code; }
    auto ok()   const -> bool { return code == OBI_OK; }
    auto fail() const -> bool { return !ok(); }

    auto reset(int num = OBI_OK)
    -> result& {
        code = num;
        message.clear();
        return *this;
    }

    auto reset(int num, std::string const& msg)
    -> result& {
        code = num;
        message = msg;
        return *this;
    }

    auto reset(int num, std::string&& msg) noexcept
    -> result& {
        code = num;
        message = std::move(msg);
        return *this;
    }

    auto reset(result const& other)
    -> result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    auto reset(result&& other) noexcept
    -> result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }

    int code;
    mutable std::string message; //lazy message assignment
};

template <typename T>
struct typed_result {
    using value_type = T;
    value_type value;
    bool valid = true;

private:
    result _result;

public:
    //// constructors
    typed_result() = default;

    // handling lvalue references and pointers
    template <bool x = std::is_lvalue_reference_v<T> ||
                       std::is_pointer_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type value
                ,result const& res = {}
                )
    :value(value)
    ,valid(true)
    ,_result(res)
    {
    #ifdef OBI_DEBUG
        std::cerr << "ctor: lvalue ref / pointer - 0 copy" << std::endl;
    #endif
    }

    template <bool x = std::is_lvalue_reference_v<T> ||
                       std::is_pointer_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type value
                ,result&& res
                )
    :value(value)
    ,valid(true)
    ,_result(std::move(res))
    {
    #ifdef OBI_DEBUG
        std::cerr << "ctor: lvalue ref / pointer - 0 copy" << std::endl;
    #endif
    }
    // handling lvalue references and pointers - end


    // handling lvalues
    template <int x = !std::is_reference_v<T> &&
                      !std::is_pointer_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type const& value
                ,result const& res = {}
                )
    :value(value) //copy here
    ,valid(true)
    ,_result(res)
    {
    #ifdef OBI_DEBUG
       std::cerr << "ctor: lvalue - 1 copy" << std::endl;
    #endif
    }

    template <int x = !std::is_reference_v<T> &&
                      !std::is_pointer_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type const& value
                ,result&& res
                )
    :value(value) //copy here
    ,valid(true)
    ,_result(std::move(res))
    {
    #ifdef OBI_DEBUG
      std::cerr << "ctor: lvalue - 1 copy" << std::endl;
    #endif
    }
    // handling lvalues - end


    // handling rvalue / copy
    template <std::uint32_t x = !std::is_reference_v<T> &&
                                !std::is_pointer_v<T> &&
                                 std::is_move_constructible_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type&& value
                ,result const& res = {}
                )
    : value(std::move(value))
    , valid(true)
    , _result(res)
    {
    #ifdef OBI_DEBUG
        std::cerr << "ctor: rvalue (move ctor) - 0 copy" << std::endl;
    #endif
    }

    template <std::uint32_t x = !std::is_reference_v<T> &&
                                !std::is_pointer_v<T> &&
                                 std::is_move_constructible_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type&& value
                ,result && res
                )
    :value(std::move(value))
    ,valid(true)
    ,_result(std::move(res))
    {
    #ifdef OBI_DEBUG
        std::cerr << "ctor: rvalue (move ctor) - 0 copy" << std::endl;
    #endif
    }
    // handling rvalue / copy - end


    // handling rvalue / assign
    template <std::uint64_t x = !std::is_reference_v<T> &&
                                !std::is_pointer_v<T> &&
                                !std::is_move_constructible_v<T> &&
                                 std::is_move_assignable_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type&& val
                ,result const& res = {}
                )
    :value()
    ,valid(true)
    ,_result(res)
    {
        this->value = std::move(val);
    #ifdef OBI_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
    #endif
    }

    template <std::uint64_t x = !std::is_reference_v<T> &&
                                !std::is_pointer_v<T> &&
                                !std::is_move_constructible_v<T> &&
                                 std::is_move_assignable_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type&& val
                ,result&& res
                )
    :value()
    ,valid(true)
    ,_result(std::move(res))
    {
        this->value = std::move(val);
    #ifdef OBI_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
    #endif
    }
    // handling rvalue / assign - end


    // forward to result's functions
    int code() const { return _result.get_code(); }
    int get_code() const { return _result.get_code(); }
    std::string message() const& { return _result.get_message(); }
    std::string message() && { return std::move(std::move(_result).get_message()); }
    std::string get_message() const& { return _result.get_message(); }
    std::string get_message() && { return std::move(std::move(_result).get_message()); }

    bool ok()   const { return _result.ok(); }
    bool fail() const { return !ok(); }
    operator bool() { return ok(); }
    bool is(int code)    const { return _result.get_code() == code; }
    bool isNot(int code) const { return !is(code); }

    // this function does not modify the value - it behaves exactly as it
    // does for the standalone result
    template <typename ...Args>
    typed_result& reset(Args&&... args) {
      _result.reset(std::forward<Args>(args)...);
      return *this;
    }

    // some functions to retrieve the internal result
    result  copy_result() const & { return _result; }             // object is lvalue
    result  copy_result()      && { return std::move(_result); }  // object is rvalue
    result  take_result()         { return std::move(_result); }  // all value types

    result&       get_result()       & { return _result; }             // get only on lvalues
    result const& get_result() const & { return _result; }             // get only on lvalues
};

inline std::ostream& operator<< (std::ostream &out, obi::util::result const& res) {
    out << std::boolalpha
        << ", ok:" << res.ok()
        << ", code:" << res.code
        << ", message:'" << res.message << "'"
        ;
    return out;
}

template <typename T>
std::ostream& operator<< (std::ostream &out, obi::util::typed_result<T> const& res) {
    out << std::boolalpha
        << ", ok:" << res.ok()
        << ", code:" << res.code()
        << ", message:'" << res.message() << "'"
        << ", vaild:" << res.valid
        ;
    return out;
}

}} // obi::util
#endif
