// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_EXCEPT_HEADER
#define EXT_UTIL_EXCEPT_HEADER

#include <stdexcept>

namespace ext { namespace util {

/*! Errors based on std::locig_error should be preventable
 *  at compile time. Errors based on std::runtime_error are
 *  not preventable.
 */

class not_implemented_exception : public std::logic_error {
    public:
    explicit not_implemented_exception() : std::logic_error("not implemented"){};
    explicit not_implemented_exception(std::string const& arg) : std::logic_error(arg){};
    explicit not_implemented_exception(char const* arg) : std::logic_error(arg){};
    not_implemented_exception(not_implemented_exception const&) = default;
    not_implemented_exception& operator=(not_implemented_exception const&) = default;
    not_implemented_exception(not_implemented_exception&&) = default;
    not_implemented_exception& operator=(not_implemented_exception&&) = default;
    virtual ~not_implemented_exception() noexcept = default;
};

class debug_exception : public std::logic_error {
    public:
    explicit debug_exception(std::string const& arg) : std::logic_error(arg){};
    explicit debug_exception(char const* arg) : std::logic_error(arg){};
    debug_exception(debug_exception const&) = default;
    debug_exception& operator=(debug_exception const&) = default;
    debug_exception(debug_exception&&) = default;
    debug_exception& operator=(debug_exception&&) = default;
    virtual ~debug_exception() noexcept = default;
};

class permission_denied_exception : public std::runtime_error {
    public:
    explicit permission_denied_exception(std::string const& arg) : std::runtime_error(arg){};
    explicit permission_denied_exception(char const* arg) : std::runtime_error(arg){};
    permission_denied_exception(permission_denied_exception const&) = default;
    permission_denied_exception& operator=(permission_denied_exception const&) = default;
    permission_denied_exception(permission_denied_exception&&) = default;
    permission_denied_exception& operator=(permission_denied_exception&&) = default;
    virtual ~permission_denied_exception() noexcept = default;
};

class cat_not_connect_exception : public std::runtime_error {
    public:
    explicit cat_not_connect_exception(std::string const& arg) : std::runtime_error(arg){};
    explicit cat_not_connect_exception(char const* arg) : std::runtime_error(arg){};
    cat_not_connect_exception(cat_not_connect_exception const&) = default;
    cat_not_connect_exception& operator=(cat_not_connect_exception const&) = default;
    cat_not_connect_exception(cat_not_connect_exception&&) = default;
    cat_not_connect_exception& operator=(cat_not_connect_exception&&) = default;
    virtual ~cat_not_connect_exception() noexcept = default;
};
}}     // namespace ext::util
#endif // EXT_UTIL_EXCEPT_HEADER
