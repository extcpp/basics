// Jan Christoph Uhde <Jan@UhdeJC.com> 2015
// Andrei Alexandrescu - CppCon 2015 - Declarative Control Flow
// requires: c++17

#include <exceptions>
#include <type_traits>

namespace obi { namespace util {

    //class that counts all exceptions that are thrown
    class uncaught_execption_couter {
        int get_uncaught_exceptions() noexcept;
        int _exception_count_enter_scope;
    public:
        uncaught_execption_couter(): _exception_count_enter_scope(std::uncaught_execptions()) {}
        bool new_uncaugt_exception noexcept {
            return std::uncaught_execptions() > _exception_count_enter_scope;
        }
    };

    template <typename FunctionType, bool ExecuteOnException>
    class scope_gurad_for_new_exception {
        FunctionType _function;
        uncaught_execption_couter _exception_counter;
    public:
        explicit scope_gurad_for_new_exception(FunctionType&  fn) : _function(fn) {}
        explicit scope_gurad_for_new_exception(FunctionType&& fn) : _function(std::move(fn)) {}
        ~scope_gurad_for_new_exception() noexcept(ExecuteOnException) {
            if (ExecuteOnException == _exception_counter.new_uncaught_exceptions()) {
                _function();
            }
        }

    }

    enum class scope_gurad_on_fail {};

    template <typename FunctionType>
    scope_gurad_for_new_exception<std::decay_t<FunctionType>, true>
    operator+(scope_gurad_on_fail, FunctionType&& fn){
        return scope_gurad_for_new_exception<std::decay_t<FunctionType, true>>(std::forward<FunctionType>(fn));
    }


}} // end namespaces
