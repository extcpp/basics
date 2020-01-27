// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_UTIL_LAZY_HEADER
#define EXT_UTIL_LAZY_HEADER
#include <type_traits>
#include <utility>

namespace ext { namespace util {


// The following struct is taken from Jean Guegant blog who invites to use it.
// see: http://jguegant.github.io/blogs/tech/
template<class Invocable>
struct lazy_construct {
    using result_type = std::invoke_result_t<const Invocable&>;

    constexpr lazy_construct(Invocable&& factory) : _factory(std::move(factory)) {}

    constexpr operator result_type() const noexcept(std::is_nothrow_invocable_v<const Invocable&>) {
        return _factory();
    }

    Invocable _factory;
};

}}     // namespace ext::util
#endif // EXT_UTIL_LAZY_HEADER
