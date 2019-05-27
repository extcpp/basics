// Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
//
// idea taken form:
// Andrei Alexandrescu - CppCon 2015 - Declarative Control Flow
// https://github.com/CppCon/CppCon2015/blob/master/Presentations/Declarative%20Control%20Flow/Declarative%20Control%20Flow%20-%20Andrei%20Alexandrescu%20-%20CppCon%202015.pdf
// https://www.youtube.com/watch?v=WjTrfoiB0MQ
//
// requires: c++17

#pragma once
#ifndef OBI_UTIL_SCOPE_GUARD_HEADER
#define OBI_UTIL_SCOPE_GUARD_HEADER
#include <exception>
#include <type_traits>
#include <cstdint>
#include <iostream> // iostream to different TU?

#include <obi/macros/general.hpp>

namespace obi { namespace util {
namespace _detail {

//! class that counts exceptions that are thrown within a scope
struct uncaught_exception_counter {
    uncaught_exception_counter()
        : exceptions_on_enter_scope(std::uncaught_exceptions())
        {}
    int exceptions_on_enter_scope;
    // function that allow to check if new exceptions occurred
    bool exception_occurred() noexcept {
        return std::uncaught_exceptions() > exceptions_on_enter_scope;
    }

    std::int32_t count() {
        if (exception_occurred()) {
            return std::uncaught_exceptions() - exceptions_on_enter_scope;
        } else {
            return std::uint32_t(0);
        }
    }
};

} //namespace _detail - end

enum class scope_guard_execution_policy {
    always,
    on_no_exception,
    on_exception
};

template <typename FunctionType
         ,scope_guard_execution_policy policy =
            scope_guard_execution_policy::always
         >
struct scope_guard {
    using decayed_type = std::decay_t<FunctionType>;
    decayed_type callback;
    _detail::uncaught_exception_counter counter;
    bool active = true;
    void deactivate(){ active = false; }

    scope_guard(FunctionType&& fn)
    noexcept(std::is_nothrow_constructible_v<decayed_type>)
        :callback(std::move_if_noexcept(fn))
        {}
    scope_guard(FunctionType const& fn)
    noexcept(std::is_copy_constructible_v<decayed_type>)
        :callback(fn)
        {}

    void execute_and_terminate_on_exception() noexcept {
        try {
            callback();
        } catch (...) {
            std::ios_base::Init ioInit;
            std::cerr << "This program will now terminate because a "
                      << "scope_gurad callback threw an execption."
                      << std::endl;
            std::terminate();
        }
    }

    void execute()
    noexcept (policy != scope_guard_execution_policy::on_no_exception) {
        if constexpr(policy == scope_guard_execution_policy::always) {
            execute_and_terminate_on_exception();
        } else if constexpr(policy == scope_guard_execution_policy::on_exception){
            if (counter.exception_occurred()){
                execute_and_terminate_on_exception();
            }
        } else /*on_no_exception*/ {
            if (!counter.exception_occurred()){
                callback();
            }
        }
    }

    ~scope_guard()
#if (defined(__GNUC__) && !defined(__clang__) && __GNUC__ <= 8)
    // noexcept (noexcept(execute())) does not work with older gcc
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1207
    noexcept (policy != scope_guard_execution_policy::on_no_exception) { // FIXME - update travis compiler
#else
    // does it really work with with cl?
    noexcept (noexcept(execute())) {
#endif // (defined(__GNUC__) && !defined(__clang__) && __GNUC__ <= 8)
        if(active) {
            execute();
        }
    }

    void* operator new(std::size_t) = delete; //stack only
    scope_guard() = delete;
    scope_guard(scope_guard const&) = delete;
    scope_guard(scope_guard &&) = delete;
};

}} // namespace ::obi::util - end

//
// FOR USE IN CPPs only (can break one-definiton-ruld in inline functions)
// __COUNTER__ would be different in each header.
//
// YOU MUST ADD A ; after the SCOPE as it closes the lambda
// ALL LAMBDAS CAPTURE BY REF!!! (use plain scope_guard as alternative)
//

#define OBI_SCOPE_FAIL \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::on_fail_helper() + [&]()

#define OBI_SCOPE_SUCCESS \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::on_success_helper() + [&]()

#define OBI_SCOPE_EXIT \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::on_exit_helper() + [&]()


namespace obi { namespace util {
namespace _detail {

//tag dispatching helper
class on_fail_helper {};
class on_success_helper {};
class on_exit_helper {};

template <typename Helper, typename FunctionType>
inline decltype(auto)
operator+(Helper, FunctionType&& fn){
    if constexpr(std::is_same_v<Helper, on_exit_helper>){
        return scope_guard<std::decay_t<FunctionType>
                         ,scope_guard_execution_policy::always
                         >
                         (std::forward<decltype(fn)>(fn));
    } else if constexpr(std::is_same_v<Helper, on_success_helper>){
        return scope_guard<std::decay_t<FunctionType>
                         ,scope_guard_execution_policy::on_no_exception
                         >
                         (std::forward<decltype(fn)>(fn));
    } else if constexpr(std::is_same_v<Helper, on_fail_helper>){
        return scope_guard<std::decay_t<FunctionType>
                         ,scope_guard_execution_policy::on_exception
                         >
                         (std::forward<decltype(fn)>(fn));
    } else {
        std::terminate();
    }
}

} // namespace _detail - end
}} // namespace ::obi::util - end
#endif // OBI_UTIL_SCOPE_GUARD_HEADER
