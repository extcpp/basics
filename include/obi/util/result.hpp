#pragma once
#ifndef OBI_UTIL_RESULT_HPP
#define OBI_UTIL_RESULT_HPP

#include <type_traits>
#include <memory>
#include <cstdint>
#include <string>

#ifdef OBI_DEBUG
    #include <iostream>
#endif

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
}
#endif
// REMOVE - END /////////////////////////////////////////////////////

namespace obi { namespace util {

namespace v1 {

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
    explicit operator bool() { return ok(); }

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
    typed_result(value_type val
                ,result const& res = {}
                )
    :value(val)
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
    typed_result(value_type val
                ,result&& res
                )
    :value(val)
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
    typed_result(value_type const& val
                ,result const& res = {}
                )
    :value(val) //copy here
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
    typed_result(value_type const& val
                ,result&& res
                )
    :value(val) //copy here
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
    typed_result(value_type&& val
                ,result const& res = {}
                )
    : value(std::move(val))
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
    typed_result(value_type&& val
                ,result && res
                )
    :value(std::move(val))
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
    explicit operator bool() { return ok() && valid; }
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

} //namespace v1

namespace v2 {

template <typename T = bool>
struct typed_result {
    using value_type = T;
    mutable std::string message; //lazy message assignment
    int code;
    value_type value;

    //// constructors
    typed_result() = default;

    // handling lvalue references and pointers
    template <bool x = std::is_lvalue_reference_v<T> ||
                       std::is_pointer_v<T>
             ,typename std::enable_if_t<x,int> = 0
             >
    typed_result(value_type val //required
                ,int co = OBI_OK
                ,std::string const& msg = ""
                )

    :message(msg)
    ,code(co)
    ,value(val)
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
    typed_result(value_type const& val
                ,int co = OBI_OK
                ,std::string const& msg = ""
                )
    :message(msg)
    ,code(co)
    ,value(val) //copy here
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
    typed_result(value_type&& val
                ,int co = OBI_OK
                ,std::string const& msg = ""
                )
    :message(msg)
    ,code(co)
    ,value(std::move(val)) //copy here
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
                ,int co = OBI_OK
                ,std::string const& msg = ""
                )
    :message(msg)
    ,code(co)
    ,value()
    {
        value = std::move(val);
    #ifdef OBI_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
    #endif
    }

    // handling rvalue / assign - end

    auto get_code() const -> int { return code; }
    auto get_message() const
    -> std::string {
        if (message.empty()) {
            if(code != OBI_OK) {
                message = error_code_vo_string(code);
            }
        }
        return message;
    }

    auto ok()   const -> bool { return code == OBI_OK; }
    auto fail() const -> bool { return !ok(); }
    explicit operator bool() { return ok(); }

    explicit operator typed_result<bool>() {
        if constexpr(std::is_same_v<value_type,bool>){
            return typed_result<bool>{value, code, message};
        } else {
            return typed_result<bool>{false, code, message};
        }
    }

    // reset does not modify vlaue
    auto reset(int num = OBI_OK)
    -> typed_result& {
        code = num;
        message.clear();
        return *this;
    }

    auto reset(int num, std::string const& msg)
    -> typed_result& {
        code = num;
        message = msg;
        return *this;
    }

    auto reset(int num, std::string&& msg) noexcept
    -> typed_result& {
        code = num;
        message = std::move(msg);
        return *this;
    }

    auto reset(typed_result const& other)
    -> typed_result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    auto reset(typed_result&& other) noexcept
    -> typed_result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }

};

using result = typed_result<bool>;

}

using namespace v1;
}} // obi::util
#endif
