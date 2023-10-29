// Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#if __cplusplus <= 201703L
static_assert(false, "C++20 required")
#endif // __cplusplus <= 201703L

#ifndef EXT_UTIL_TUPLE_HEADER
    #define EXT_UTIL_TUPLE_HEADER

    #include <ext/meta/basic.hpp>

    #include <tuple>
    #include <type_traits>
    #include <utility>

    namespace ext::util {

    // is tuple check
    template <typename T, typename = void>
    struct is_tuple_impl : std::false_type {};
    template <typename... T>
    struct is_tuple_impl<std::tuple<T...>> : std::true_type {};
    template <typename T>
    struct is_tuple : is_tuple_impl<meta::remove_cvref_t<T>> {};
    template <typename T>
    constexpr bool is_tuple_v = is_tuple<T>::value;

    // for each impl
    template <typename Tuple, typename F>
    constexpr decltype(auto) tuple_for_each(Tuple && tuple, F && f) {
        static_assert(is_tuple_v<Tuple>);
        constexpr std::size_t size = std::tuple_size_v<meta::remove_cvref_t<Tuple>>;

        return []<std::size_t... I>(Tuple && _tuple, F && _f, std::index_sequence<I...>) {
            (..., _f(std::get<I>(_tuple)));
            return _f; // this can be meaningful if f is a functor that has now a changed state
        }
        (std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<size>{});
    }

} // namespace ext::util
#endif // EXT_UTIL_TUPLE_HEADER
