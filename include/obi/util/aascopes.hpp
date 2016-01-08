// Jan Christoph Uhde <Jan@UhdeJC.com> 2015
//
// idea taken form:
// Andrei Alexandrescu - CppCon 2015 - Declarative Control Flow
// https://github.com/CppCon/CppCon2015/blob/master/Presentations/Declarative%20Control%20Flow/Declarative%20Control%20Flow%20-%20Andrei%20Alexandrescu%20-%20CppCon%202015.pdf
// https://www.youtube.com/watch?v=WjTrfoiB0MQ
//
// requires: c++17

#include <exception>
#include <type_traits>
#include "../macros.hpp"

//
// YOU MUST ADD A ; after the SCOPE as it closes the lambda
//
#define OBI_SCOPE_FAIL \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::scope_guard_on_fail() + [&]() noexcept

#define OBI_SCOPE_SUCCESS \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::scope_guard_on_success() + [&]() noexcept

#define OBI_SCOPE_EXIT \
    auto OBI_ANONYMOUS_VARIABLE(OBI_SCOPE_FAIL_STATE) \
    = obi::util::_detail::scope_guard_on_exit() + [&]() noexcept


namespace obi { namespace util { namespace _detail {

    // RAII class that counts exceptions that are thrown within a scope
    class uncaught_exception_counter {
        int get_uncaught_exceptions() noexcept;
        int _exception_count_enter_scope; // number of exceptions at scope enter
    public:
        // functions that allow to check if new exceptions occured
        uncaught_exception_counter(): _exception_count_enter_scope(std::uncaught_exceptions() ) {}
        bool new_uncaught_exception() noexcept {
            return std::uncaught_exceptions() > _exception_count_enter_scope;
        };
    }; // uncaught_exception_counter



    // RAII class that provides mechanism for executing a function on scope exit
    // depending on exception status (fail / success)
    template <typename FunctionType, bool ExecuteOnException>
    class scope_guard_for_exception {
        FunctionType _function;
        uncaught_exception_counter _exception_counter;
    public:
        explicit scope_guard_for_exception(FunctionType&  fn) : _function(fn) {}
        explicit scope_guard_for_exception(FunctionType&& fn) : _function(std::move(fn)) {}
        // scopguard checks on destruction if exceptions occured or not and desides
        // on template parameter (ExecuteOnException) if a given function should be
        // executed
        ~scope_guard_for_exception() noexcept(ExecuteOnException) {
            if (ExecuteOnException == _exception_counter.new_uncaught_exception()) {
                _function();
            }
        }
    }; // scope_guard_for_exception - end

    // scope fail
    class scope_guard_on_fail {};
    template <typename FunctionType>
    scope_guard_for_exception<std::decay_t<FunctionType>, true>
    operator+(scope_guard_on_fail, FunctionType&& fn){
        return scope_guard_for_exception<std::decay_t<FunctionType>, true>(std::forward<FunctionType>(fn));
    }

    // scope success
    class scope_guard_on_success {};
    template <typename FunctionType>
    scope_guard_for_exception<std::decay_t<FunctionType>, false>
    operator+(scope_guard_on_success, FunctionType&& fn){
        return scope_guard_for_exception<std::decay_t<FunctionType>, false>(std::forward<FunctionType>(fn));
    }



    // equivalent to scope_guard_for_exception
    template <typename FunctionType>
    class scope_guard_for_exit {
        FunctionType _function;
    public:
        explicit scope_guard_for_exit(FunctionType&  fn) : _function(fn) {}
        explicit scope_guard_for_exit(FunctionType&& fn) : _function(std::move(fn)) {}
        ~scope_guard_for_exit() noexcept {
            _function();
        }
    }; // scope_guard_for_exit - end

    // scope exit
    class scope_guard_on_exit {};
    template <typename FunctionType>
    scope_guard_for_exit<std::decay_t<FunctionType>>
    operator+(scope_guard_on_exit, FunctionType&& fn) {
          return scope_guard_for_exit<std::decay_t<FunctionType>>(std::forward<FunctionType>(fn));
    }

}}} // end namespaces
