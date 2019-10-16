// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_RESULT_HEADER
#define EXT_UTIL_RESULT_HEADER

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#ifdef EXT_DEBUG
    #include <iostream>
#endif // EXT_DEBUG

namespace ext { namespace util {

inline int EXT_OK = 0;
inline int EXT_FAIL = 1;
inline int EXT_ERROR_NET = 10;
}} // namespace ext::util

// REMOVE ///////////////////////////////////////////////////////////
#define EXT_RESULT_NOT_FINISHED
#ifdef EXT_RESULT_NOT_FINISHED
    #include <map>
// should be in cpp
inline std::string error_code_vo_string(int code) {
    static const std::map<int, std::string> error_map = {{ext::util::EXT_ERROR_NET, "network error"}};

    auto found = error_map.find(code);
    if (found != error_map.end()) {
        return found->second;
    } else {
        return std::string();
    }
}
#endif // EXT_RESULT_NOT_FINISHED
// REMOVE - END /////////////////////////////////////////////////////

namespace ext { namespace util {

inline namespace v1 {
struct result {
    result() noexcept : code(EXT_OK) {}

    result(int num) noexcept : code(num) {}

    result(int num, std::string const& msg) : code(num), message(msg) {}

    result(int num, std::string&& msg) : code(num), message(std::move(msg)) {}

    result(result const& other) : code(other.code), message(other.message) {}

    result(result&& other) noexcept : code(other.code), message(std::move(other.message)) {}

    auto operator=(result const& other) -> result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    auto operator=(result&& other) noexcept -> result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }

    auto get_message() const -> std::string {
        if (message.empty()) {
            if (code != EXT_OK) {
                message = error_code_vo_string(code);
            }
        }
        return message;
    }

    auto get_code() const -> int {
        return code;
    }
    auto ok() const -> bool {
        return code == EXT_OK;
    }
    auto fail() const -> bool {
        return !ok();
    }
    explicit operator bool() {
        return ok();
    }

    auto reset(int num = EXT_OK) -> result& {
        code = num;
        message.clear();
        return *this;
    }

    auto reset(int num, std::string const& msg) -> result& {
        code = num;
        message = msg;
        return *this;
    }

    auto reset(int num, std::string&& msg) noexcept -> result& {
        code = num;
        message = std::move(msg);
        return *this;
    }

    auto reset(result const& other) -> result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    auto reset(result&& other) noexcept -> result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }

    int code;
    mutable std::string message; // lazy message assignment
};

template<typename T>
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
    template<bool x = std::is_lvalue_reference_v<T> || std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type val, result const& res = {}) : value(val), valid(true), _result(res) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue ref / pointer - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<bool x = std::is_lvalue_reference_v<T> || std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type val, result&& res) : value(val), valid(true), _result(std::move(res)) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue ref / pointer - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }
    // handling lvalue references and pointers - end

    // handling lvalues
    template<int x = !std::is_reference_v<T> && !std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type const& val, result const& res = {})
        : value(val) // copy here
        , valid(true)
        , _result(res) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue - 1 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<int x = !std::is_reference_v<T> && !std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type const& val, result&& res)
        : value(val) // copy here
        , valid(true)
        , _result(std::move(res)) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue - 1 copy" << std::endl;
#endif // EXT_DEBUG
    }
    // handling lvalues - end

    // handling rvalue / copy
    template<std::uint32_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && std::is_move_constructible_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, result const& res = {}) : value(std::move(val)), valid(true), _result(res) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move ctor) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<std::uint32_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && std::is_move_constructible_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, result&& res) : value(std::move(val)), valid(true), _result(std::move(res)) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move ctor) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }
    // handling rvalue / copy - end

    // handling rvalue / assign
    template<std::uint64_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && !std::is_move_constructible_v<T> &&
                               std::is_move_assignable_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, result const& res = {}) : value(), valid(true), _result(res) {
        this->value = std::move(val);
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<std::uint64_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && !std::is_move_constructible_v<T> &&
                               std::is_move_assignable_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, result&& res) : value(), valid(true), _result(std::move(res)) {
        this->value = std::move(val);
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }
    // handling rvalue / assign - end

    // forward to result's functions
    int code() const {
        return _result.get_code();
    }
    int get_code() const {
        return _result.get_code();
    }
    std::string message() const& {
        return _result.get_message();
    }
    std::string message() && {
        return std::move(std::move(_result).get_message());
    }
    std::string get_message() const& {
        return _result.get_message();
    }
    std::string get_message() && {
        return std::move(std::move(_result).get_message());
    }

    bool ok() const {
        return _result.ok();
    }
    bool fail() const {
        return !ok();
    }
    explicit operator bool() {
        return ok() && valid;
    }
    bool is(int code) const {
        return _result.get_code() == code;
    }
    bool isNot(int code) const {
        return !is(code);
    }

    // this function does not modify the value - it behaves exactly as it
    // does for the standalone result
    template<typename... Args>
    typed_result& reset(Args&&... args) {
        _result.reset(std::forward<Args>(args)...);
        return *this;
    }

    // some functions to retrieve the internal result
    result copy_result() const& {
        return _result;
    } // object is lvalue
    result copy_result() && {
        return std::move(_result);
    } // object is rvalue
    result take_result() {
        return std::move(_result);
    } // all value types

    result& get_result() & {
        return _result;
    } // get only on lvalues
    result const& get_result() const& {
        return _result;
    } // get only on lvalues
};

} // namespace v1

namespace v2 {
// WIP
// TODO
// - needs constructors taking result<bool>
// - probaly should have static error / success functions
//   always passing {} is odd
//
// typed_result<bool> success() { return typed_result<bool>(); }
// typed_result<bool> fail() { return typed_result<bool>(); }
//
// what is the best way to create one typed_result from another
// with a different T?
// typed_result<A>({},result(std::move(result_of_type_B)))
//
// template<typename X>
// typed_result::reset(typed_result<X>&& res)

template<typename T = bool>
struct typed_result {
    using value_type = T;
    mutable std::string message; // lazy message assignment
    int code;
    value_type value;

    //// constructors
    typed_result() = default;

    // handling lvalue references and pointers
    template<bool x = std::is_lvalue_reference_v<T> || std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type val // required
                 ,
                 int co = EXT_OK,
                 std::string const& msg = "")

        : message(msg), code(co), value(val) {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue ref / pointer - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    // handling lvalue references and pointers - end

    // handling lvalues
    template<int x = !std::is_reference_v<T> && !std::is_pointer_v<T>, typename std::enable_if_t<x, int> = 0>
    typed_result(value_type const& val, int co = EXT_OK, std::string const& msg = "")
        : message(msg)
        , code(co)
        , value(val) // copy here
    {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue - 1 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<typename V,
             int x = !std::is_reference_v<T> && !std::is_pointer_v<T> && !std::is_same_v<V, T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type const& val, typed_result<V>&& res)
        : message(std::move(res.message))
        , code(res.code)
        , value(val) // copy here
    {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue - 1 copy" << std::endl;
#endif // EXT_DEBUG
    }

    template<typename V,
             int x = !std::is_reference_v<T> && !std::is_pointer_v<T> && std::is_same_v<V, T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(typed_result<V> const& res)
        : message(res.message)
        , code(res.code)
        , value(res.value) // copy here
    {
#ifdef EXT_DEBUG
        std::cerr << "ctor: lvalue - 1 copy" << std::endl;
#endif // EXT_DEBUG
    }
    // handling lvalues - end

    // handling rvalue / copy
    template<std::uint32_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && std::is_move_constructible_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, int co = EXT_OK, std::string const& msg = "")
        : message(msg)
        , code(co)
        , value(std::move(val)) // copy here
    {
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move ctor) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    // handling rvalue / copy - end

    // handling rvalue / assign
    template<std::uint64_t x = !std::is_reference_v<T> && !std::is_pointer_v<T> && !std::is_move_constructible_v<T> &&
                               std::is_move_assignable_v<T>,
             typename std::enable_if_t<x, int> = 0>
    typed_result(value_type&& val, int co = EXT_OK, std::string const& msg = "") : message(msg), code(co), value() {
        value = std::move(val);
#ifdef EXT_DEBUG
        std::cerr << "ctor: rvalue (move assign) - 0 copy" << std::endl;
#endif // EXT_DEBUG
    }

    // handling rvalue / assign - end

    auto get_code() const -> int {
        return code;
    }
    auto get_message() const -> std::string {
        if (message.empty()) {
            if (code != EXT_OK) {
                message = error_code_vo_string(code);
            }
        }
        return message;
    }

    auto ok() const -> bool {
        return code == EXT_OK;
    }
    auto fail() const -> bool {
        return !ok();
    }
    explicit operator bool() {
        return ok();
    }

    explicit operator typed_result<bool>() const& {
        if constexpr (std::is_same_v<value_type, bool>) {
            return typed_result<bool>{value, code, message};
        } else {
            return typed_result<bool>{false, code, message};
        }
    }

    explicit operator typed_result<bool>() && {
        if constexpr (std::is_same_v<value_type, bool>) {
            return typed_result<bool>{value, code, std::move(message)};
        } else {
            return typed_result<bool>{false, code, std::move(message)};
        }
    }

    typed_result<bool> to_result() const& {
        if constexpr (std::is_same_v<value_type, bool>) {
            return typed_result<bool>{value, code, message};
        } else {
            return typed_result<bool>{false, code, message};
        }
    }

    typed_result<bool> to_result() && {
        if constexpr (std::is_same_v<value_type, bool>) {
            return typed_result<bool>{value, code, std::move(message)};
        } else {
            return typed_result<bool>{false, code, std::move(message)};
        }
    }

    typed_result success() {
        return typed_result({});
    }
    typed_result error(int co = EXT_FAIL, std::string mes = "") {
        return typed_result({}, co, mes);
    }
    typed_result error(std::string mes = "", int co = EXT_FAIL) {
        return typed_result({}, co, mes);
    }

    // reset does not modify vlaue
    auto reset(int num = EXT_OK) -> typed_result& {
        code = num;
        message.clear();
        return *this;
    }

    auto reset(int num, std::string const& msg) -> typed_result& {
        code = num;
        message = msg;
        return *this;
    }

    auto reset(int num, std::string&& msg) noexcept -> typed_result& {
        code = num;
        message = std::move(msg);
        return *this;
    }

    template<typename V>
    auto reset(typed_result<V> const& other) -> typed_result& {
        code = other.code;
        message = other.message;
        return *this;
    }

    template<typename V>
    auto reset(typed_result<V>&& other) noexcept -> typed_result& {
        code = other.code;
        message = std::move(other.message);
        return *this;
    }
};

using result = typed_result<bool>;
} // namespace v2
}}     // namespace ext::util
#endif // EXT_UTIL_RESULT_HEADER
