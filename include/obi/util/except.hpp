// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_EXCEPT_HEADER
#define OBI_UTIL_EXCEPT_HEADER

#include <stdexcept>

namespace obi { namespace util {

  /*! Errors based on std::locig_error should be preventable
   *  at compile time. Errors based on std::runtime_error are
   *  not preventable.
   */

  class not_implemented_exception : public std::logic_error {
  public:
    explicit not_implemented_exception() : std::logic_error("not implemented"){};
    explicit not_implemented_exception(std::string const& arg);
    explicit not_implemented_exception(char const*);
    not_implemented_exception(not_implemented_exception const&) = default;
    not_implemented_exception& operator=(not_implemented_exception const&) = default;
    not_implemented_exception(not_implemented_exception&&) = default;
    not_implemented_exception& operator=(not_implemented_exception&&) = default;
    virtual ~not_implemented_exception() noexcept;
  };

  class debug_exception : public std::logic_error {
  public:
    explicit debug_exception(std::string const& arg);
    explicit debug_exception(char const*);
    debug_exception(debug_exception const&) = default;
    debug_exception& operator=(debug_exception const&) = default;
    debug_exception(debug_exception&&) = default;
    debug_exception& operator=(debug_exception&&) = default;
    virtual ~debug_exception() noexcept;
  };

  class permission_denied_exception : public std::runtime_error {
  public:
    explicit permission_denied_exception(std::string const& arg);
    explicit permission_denied_exception(char const*);
    permission_denied_exception(permission_denied_exception const&) = default;
    permission_denied_exception& operator=(permission_denied_exception const&) = default;
    permission_denied_exception(permission_denied_exception&&) = default;
    permission_denied_exception& operator=(permission_denied_exception&&) = default;
    virtual ~permission_denied_exception() noexcept;
  };

  class cat_not_connect_exception : public std::runtime_error {
  public:
    explicit cat_not_connect_exception(std::string const& arg);
    explicit cat_not_connect_exception(char const*);
    cat_not_connect_exception(cat_not_connect_exception const&) = default;
    cat_not_connect_exception& operator=(cat_not_connect_exception const&) = default;
    cat_not_connect_exception(cat_not_connect_exception&&) = default;
    cat_not_connect_exception& operator=(cat_not_connect_exception&&) = default;
    virtual ~cat_not_connect_exception() noexcept;
  };

}} // obi::util
#endif // OBI_UTIL_EXCEPT_HEADER