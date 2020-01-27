///
// expected - An implementation of std::expected with extensions
// Written in 2017 by Simon Brand (simonrbrand@gmail.com, @TartanLlama)
//
// Documentation available at http://tl.tartanllama.xyz/
//
// To the extent possible under law, the author(s) have dedicated all
// copyright and related and neighboring rights to this software to the
// public domain worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication
// along with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
// https://github.com/TartanLlama/expected/tree/1d9c5d8c0da84b8ddc54bd3d90d632eec95c1f13
///

// Copyright - 2019-2020 - Jan Christoph Uhde <Jan@UhdeJC.com> -- added by check_code
#ifndef EXT_UTIL_EXPECTED_HEADER
#define EXT_UTIL_EXPECTED_HEADER
#include <exception>
#include <functional>
#include <type_traits>
#include <utility>
#include <variant>

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
    #define EXT_EXPECTED_EXCEPTIONS_ENABLED
#endif // defined(__EXCEPTIONS) || defined(_CPPUNWIND)


namespace ext::util {
template<class T, class E>
class expected;

template<class E>
class unexpected {
    public:
    static_assert(!std::is_same_v<E, void>, "E must not be void");

    unexpected() = delete;
    constexpr explicit unexpected(const E& e) : m_val(e) {}

    constexpr explicit unexpected(E&& e) : m_val(std::move(e)) {}

    constexpr const E& value() const& {
        return m_val;
    }
    constexpr E& value() & {
        return m_val;
    }
    constexpr E&& value() && {
        return std::move(m_val);
    }
    constexpr const E&& value() const&& {
        return std::move(m_val);
    }

    private:
    E m_val;
};

template<class E>
constexpr bool operator==(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() == rhs.value();
}
template<class E>
constexpr bool operator!=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() != rhs.value();
}
template<class E>
constexpr bool operator<(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() < rhs.value();
}
template<class E>
constexpr bool operator<=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() <= rhs.value();
}
template<class E>
constexpr bool operator>(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() > rhs.value();
}
template<class E>
constexpr bool operator>=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
    return lhs.value() >= rhs.value();
}

template<class E>
unexpected<typename std::decay<E>::type> make_unexpected(E&& e) {
    return unexpected<typename std::decay<E>::type>(std::forward<E>(e));
}

struct unexpect_t {
    unexpect_t() = default;
};
static constexpr unexpect_t unexpect{};

namespace detail {
template<typename E>
[[noreturn]] constexpr void throw_exception(E&& e) {
#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
    throw std::forward<E>(e);
#else
    (void) e;
    #ifdef _MSC_VER
    __assume(0);
    #else
    __builtin_unreachable();
    #endif
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
}

// https://stackoverflow.com/questions/26744589/what-is-a-proper-way-to-implement-is-swappable-to-test-for-the-swappable-concept
namespace swap_adl_tests {
// if swap ADL finds this then it would call std::swap otherwise (same signature)
struct tag {};

template<class T>
tag swap(T&, T&);
template<class T, std::size_t N>
tag swap(T (&a)[N], T (&b)[N]);

// helper functions to test if an unqualified swap is possible, and if it
// becomes std::swap
template<class, class>
std::false_type can_swap(...) noexcept(false);
template<class T, class U, class = decltype(swap(std::declval<T&>(), std::declval<U&>()))>
std::true_type can_swap(int) noexcept(noexcept(swap(std::declval<T&>(), std::declval<U&>())));

template<class, class>
std::false_type uses_std(...);
template<class T, class U>
std::is_same<decltype(swap(std::declval<T&>(), std::declval<U&>())), tag> uses_std(int);

template<class T>
struct is_std_swap_noexcept
    : std::integral_constant<bool, std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>> {};

template<class T, std::size_t N>
struct is_std_swap_noexcept<T[N]> : is_std_swap_noexcept<T> {};

template<class T, class U>
struct is_adl_swap_noexcept : std::bool_constant<noexcept(can_swap<T, U>(0))> {};
} // namespace swap_adl_tests

template<class T, class U = T>
struct is_swappable
    : std::integral_constant<bool,
                             decltype(detail::swap_adl_tests::can_swap<T, U>(0))::value &&
                                 (!decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value ||
                                  (std::is_move_assignable_v<T> && std::is_move_constructible_v<T>) )> {};

template<class T, std::size_t N>
struct is_swappable<T[N], T[N]>
    : std::integral_constant<bool,
                             decltype(detail::swap_adl_tests::can_swap<T[N], T[N]>(0))::value &&
                                 (!decltype(detail::swap_adl_tests::uses_std<T[N], T[N]>(0))::value ||
                                  is_swappable<T, T>::value)> {};

template<class T, class U = T>
struct is_nothrow_swappable
    : std::integral_constant<bool,
                             is_swappable<T, U>::value &&
                                 ((decltype(detail::swap_adl_tests::uses_std<T, U>(
                                      0))::value&& detail::swap_adl_tests::is_std_swap_noexcept<T>::value) ||
                                  (!decltype(detail::swap_adl_tests::uses_std<T, U>(
                                      0))::value&& detail::swap_adl_tests::is_adl_swap_noexcept<T, U>::value))> {};

// Trait for checking if a type is a tl::expected
template<class T>
struct is_expected_impl : std::false_type {};
template<class T, class E>
struct is_expected_impl<expected<T, E>> : std::true_type {};
template<class T>
using is_expected = is_expected_impl<std::decay_t<T>>;
template<class T>
constexpr bool is_expected_v = is_expected<T>::value;

template<class T, class E, class U>
using expected_enable_forward_value =
    std::enable_if_t<std::is_constructible_v<T, U&&> && !std::is_same_v<std::decay_t<U>, std::in_place_t> &&
                     !std::is_same_v<expected<T, E>, std::decay_t<U>> &&
                     !std::is_same_v<unexpected<E>, std::decay_t<U>>>;

template<class T, class E, class U, class G, class UR, class GR>
using expected_enable_from_other = std::enable_if_t<
    // clang-format off
    std::is_constructible_v<T, UR> &&
    std::is_constructible_v<E, GR> &&
    !std::is_constructible_v<T, expected<U, G>&> &&
    !std::is_constructible_v<T, expected<U, G>&&> &&
    !std::is_constructible_v<T, const expected<U, G>&> &&
    !std::is_constructible_v<T, const expected<U, G>&&> &&
    !std::is_convertible_v<expected<U, G>&, T> &&
    !std::is_convertible_v<expected<U, G>&&, T> &&
    !std::is_convertible_v<const expected<U, G>&, T> &&
    !std::is_convertible_v<const expected<U, G>&&, T>
    >;
// clang-format on

template<class T, class U>
using is_void_or = std::conditional_t<std::is_void_v<T>, std::true_type, U>;

template<class T, class BoolConstant>
constexpr bool is_void_or_v = std::conditional_t<std::is_void_v<T>, std::true_type, BoolConstant>::value;

template<class T>
constexpr bool is_copy_constructible_or_void_v = is_void_or_v<T, std::is_copy_constructible<T>>;

template<class T>
constexpr bool is_move_constructible_or_void_v = is_void_or_v<T, std::is_move_constructible<T>>;

template<class T>
constexpr bool is_copy_assignable_or_void_v = is_void_or_v<T, std::is_copy_assignable<T>>;

template<class T>
constexpr bool is_move_assignable_or_void_v = is_void_or_v<T, std::is_move_assignable<T>>;


} // namespace detail

namespace detail {
struct no_init_t {};
static constexpr no_init_t no_init{};

// Implements the storage of the values, and ensures that the destructor is
// trivial if it can be.
//
// This specialization is for where neither `T` or `E` is trivially
// destructible, so the destructors must be called on destruction of the
// `expected`
template<class T, class E, bool = std::is_trivially_destructible_v<T>, bool = std::is_trivially_destructible_v<E>>
struct expected_storage_base {
    constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, Args&&... args)
        : m_val(std::forward<Args>(args)...), m_has_val(true) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() {
        if (m_has_val) {
            m_val.~T();
        } else {
            m_unexpect.~unexpected<E>();
        }
    }

    union {
        T m_val;
        unexpected<E> m_unexpect;
        char m_no_init;
    };

    bool m_has_val;
};

// This specialization is for when both `T` and `E` are trivially-destructible,
// so the destructor of the `expected` can be trivial.
template<class T, class E>
struct expected_storage_base<T, E, true, true> {
    constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, Args&&... args)
        : m_val(std::forward<Args>(args)...), m_has_val(true) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() = default;
    union {
        T m_val;
        unexpected<E> m_unexpect;
        char m_no_init;
    };
    bool m_has_val;
};

// T is trivial, E is not.
template<class T, class E>
struct expected_storage_base<T, E, true, false> {
    constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, Args&&... args)
        : m_val(std::forward<Args>(args)...), m_has_val(true) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() {
        if (!m_has_val) {
            m_unexpect.~unexpected<E>();
        }
    }

    union {
        T m_val;
        unexpected<E> m_unexpect;
        char m_no_init;
    };
    bool m_has_val;
};

// E is trivial, T is not.
template<class T, class E>
struct expected_storage_base<T, E, false, true> {
    constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, Args&&... args)
        : m_val(std::forward<Args>(args)...), m_has_val(true) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr expected_storage_base(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() {
        if (m_has_val) {
            m_val.~T();
        }
    }
    union {
        T m_val;
        unexpected<E> m_unexpect;
        char m_no_init;
    };
    bool m_has_val;
};

// `T` is `void`, `E` is trivially-destructible
template<class E>
struct expected_storage_base<void, E, false, true> {
    constexpr expected_storage_base() : m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_val(), m_has_val(false) {}

    constexpr expected_storage_base(std::in_place_t) : m_has_val(true) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() = default;
    struct dummy {};
    union {
        unexpected<E> m_unexpect;
        dummy m_val;
    };
    bool m_has_val;
};

// `T` is `void`, `E` is not trivially-destructible
template<class E>
struct expected_storage_base<void, E, false, false> {
    constexpr expected_storage_base() : m_dummy(), m_has_val(true) {}
    constexpr expected_storage_base(no_init_t) : m_dummy(), m_has_val(false) {}

    constexpr expected_storage_base(std::in_place_t) : m_dummy(), m_has_val(true) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, Args&&... args)
        : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected_storage_base(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

    ~expected_storage_base() {
        if (!m_has_val) {
            m_unexpect.~unexpected<E>();
        }
    }

    union {
        unexpected<E> m_unexpect;
        char m_dummy;
    };
    bool m_has_val;
};

// This base class provides some handy member functions which can be used in
// further derived classes
template<class T, class E>
struct expected_operations_base : expected_storage_base<T, E> {
    using expected_storage_base<T, E>::expected_storage_base;

    template<class... Args>
    void construct(Args&&... args) noexcept {
        new (std::addressof(this->m_val)) T(std::forward<Args>(args)...);
        this->m_has_val = true;
    }

    template<class Rhs>
    void construct_with(Rhs&& rhs) noexcept {
        new (std::addressof(this->m_val)) T(std::forward<Rhs>(rhs).get());
        this->m_has_val = true;
    }

    template<class... Args>
    void construct_error(Args&&... args) noexcept {
        new (std::addressof(this->m_unexpect)) unexpected<E>(std::forward<Args>(args)...);
        this->m_has_val = false;
    }

#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED

    // These assign overloads ensure that the most efficient assignment
    // implementation is used while maintaining the strong exception guarantee.
    // The problematic case is where rhs has a value, but *this does not.
    //
    // This overload handles the case where we can just copy-construct `T`
    // directly into place without throwing.
    template<class U = T, std::enable_if_t<std::is_nothrow_copy_constructible_v<U>>* = nullptr>
    void assign(const expected_operations_base& rhs) noexcept {
        if (!this->m_has_val && rhs.m_has_val) {
            geterr().~unexpected<E>();
            construct(rhs.get());
        } else {
            assign_common(rhs);
        }
    }

    // This overload handles the case where we can attempt to create a copy of
    // `T`, then no-throw move it into place if the copy was successful.
    template<class U = T,
             std::enable_if_t<!std::is_nothrow_copy_constructible_v<U> && std::is_nothrow_move_constructible_v<U>>* =
                 nullptr>
    void assign(const expected_operations_base& rhs) noexcept {
        if (!this->m_has_val && rhs.m_has_val) {
            T tmp = rhs.get();
            geterr().~unexpected<E>();
            construct(std::move(tmp));
        } else {
            assign_common(rhs);
        }
    }

    // This overload is the worst-case, where we have to move-construct the
    // unexpected value into temporary storage, then try to copy the T into place.
    // If the construction succeeds, then everything is fine, but if it throws,
    // then we move the old unexpected value back into place before rethrowing the
    // exception.
    template<class U = T,
             std::enable_if_t<!std::is_nothrow_copy_constructible_v<U> && !std::is_nothrow_move_constructible_v<U>>* =
                 nullptr>
    void assign(const expected_operations_base& rhs) {
        if (!this->m_has_val && rhs.m_has_val) {
            auto tmp = std::move(geterr());
            geterr().~unexpected<E>();

    #ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
            try {
                construct(rhs.get());
            } catch (...) {
                geterr() = std::move(tmp);
                throw;
            }
    #else
            construct(rhs.get());
    #endif
        } else {
            assign_common(rhs);
        }
    }

    // These overloads do the same as above, but for rvalues
    template<class U = T, std::enable_if_t<std::is_nothrow_move_constructible_v<U>>* = nullptr>
    void assign(expected_operations_base&& rhs) noexcept {
        if (!this->m_has_val && rhs.m_has_val) {
            geterr().~unexpected<E>();
            construct(std::move(rhs).get());
        } else {
            assign_common(std::move(rhs));
        }
    }

    template<class U = T, std::enable_if_t<!std::is_nothrow_move_constructible_v<U>>* = nullptr>
    void assign(expected_operations_base&& rhs) {
        if (!this->m_has_val && rhs.m_has_val) {
            auto tmp = std::move(geterr());
            geterr().~unexpected<E>();
    #ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
            try {
                construct(std::move(rhs).get());
            } catch (...) {
                geterr() = std::move(tmp);
                throw;
            }
    #else
            construct(std::move(rhs).get());
    #endif
        } else {
            assign_common(std::move(rhs));
        }
    }

#else

    // If exceptions are disabled then we can just copy-construct
    void assign(const expected_operations_base& rhs) noexcept {
        if (!this->m_has_val && rhs.m_has_val) {
            geterr().~unexpected<E>();
            construct(rhs.get());
        } else {
            assign_common(rhs);
        }
    }

    void assign(expected_operations_base&& rhs) noexcept {
        if (!this->m_has_val && rhs.m_has_val) {
            geterr().~unexpected<E>();
            construct(std::move(rhs).get());
        } else {
            assign_common(rhs);
        }
    }

#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED

    // The common part of move/copy assigning
    template<class Rhs>
    void assign_common(Rhs&& rhs) {
        if (this->m_has_val) {
            if (rhs.m_has_val) {
                get() = std::forward<Rhs>(rhs).get();
            } else {
                destroy_val();
                construct_error(std::forward<Rhs>(rhs).geterr());
            }
        } else {
            if (!rhs.m_has_val) {
                geterr() = std::forward<Rhs>(rhs).geterr();
            }
        }
    }

    bool has_value() const {
        return this->m_has_val;
    }

    constexpr T& get() & {
        return this->m_val;
    }
    constexpr const T& get() const& {
        return this->m_val;
    }
    constexpr T&& get() && {
        return std::move(this->m_val);
    }
    constexpr const T&& get() const&& {
        return std::move(this->m_val);
    }

    constexpr unexpected<E>& geterr() & {
        return this->m_unexpect;
    }
    constexpr const unexpected<E>& geterr() const& {
        return this->m_unexpect;
    }
    constexpr unexpected<E>&& geterr() && {
        return std::move(this->m_unexpect);
    }
    constexpr const unexpected<E>&& geterr() const&& {
        return std::move(this->m_unexpect);
    }

    constexpr void destroy_val() {
        get().~T();
    }
};

// This base class provides some handy member functions which can be used in
// further derived classes
template<class E>
struct expected_operations_base<void, E> : expected_storage_base<void, E> {
    using expected_storage_base<void, E>::expected_storage_base;

    template<class... Args>
    void construct() noexcept {
        this->m_has_val = true;
    }

    // This function doesn't use its argument, but needs it so that code in
    // levels above this can work independently of whether T is void
    template<class Rhs>
    void construct_with(Rhs&&) noexcept {
        this->m_has_val = true;
    }

    template<class... Args>
    void construct_error(Args&&... args) noexcept {
        new (std::addressof(this->m_unexpect)) unexpected<E>(std::forward<Args>(args)...);
        this->m_has_val = false;
    }

    template<class Rhs>
    void assign(Rhs&& rhs) noexcept {
        if (!this->m_has_val) {
            if (rhs.m_has_val) {
                geterr().~unexpected<E>();
                construct();
            } else {
                geterr() = std::forward<Rhs>(rhs).geterr();
            }
        } else {
            if (!rhs.m_has_val) {
                construct_error(std::forward<Rhs>(rhs).geterr());
            }
        }
    }

    bool has_value() const {
        return this->m_has_val;
    }

    constexpr unexpected<E>& geterr() & {
        return this->m_unexpect;
    }
    constexpr const unexpected<E>& geterr() const& {
        return this->m_unexpect;
    }
    constexpr unexpected<E>&& geterr() && {
        return std::move(this->m_unexpect);
    }
    constexpr const unexpected<E>&& geterr() const&& {
        return std::move(this->m_unexpect);
    }

    constexpr void destroy_val() {
        // no-op
    }
};

// This class manages conditionally having a trivial copy constructor
// This specialization is for when T and E are trivially copy constructible
template<class T,
         class E,
         // clang-format off
         bool = is_void_or_v<T, std::is_trivially_copy_constructible<T>> &&
                std::is_trivially_copy_constructible_v<E>
         // clang-format on
         >
struct expected_copy_base : expected_operations_base<T, E> {
    using expected_operations_base<T, E>::expected_operations_base;
};

// This specialization is for when T or E are not trivially copy constructible
template<class T, class E>
struct expected_copy_base<T, E, false> : expected_operations_base<T, E> {
    using expected_operations_base<T, E>::expected_operations_base;

    expected_copy_base() = default;
    expected_copy_base(const expected_copy_base& rhs) : expected_operations_base<T, E>(no_init) {
        if (rhs.has_value()) {
            this->construct_with(rhs);
        } else {
            this->construct_error(rhs.geterr());
        }
    }

    expected_copy_base(expected_copy_base&& rhs) = default;
    expected_copy_base& operator=(const expected_copy_base& rhs) = default;
    expected_copy_base& operator=(expected_copy_base&& rhs) = default;
};

// This class manages conditionally having a trivial move constructor
// Unfortunately there's no way to achieve this in GCC < 5 AFAIK, since it
// doesn't implement an analogue to std::is_trivially_move_constructible. We
// have to make do with a non-trivial move constructor even if T is trivially
// move constructible
template<class T,
         class E,
         // clang-format off
         bool = is_void_or_v<T, std::is_trivially_move_constructible<T>> &&
                std::is_trivially_move_constructible_v<E>
         // clang-format on
         >
struct expected_move_base : expected_copy_base<T, E> {
    using expected_copy_base<T, E>::expected_copy_base;
};

template<class T, class E>
struct expected_move_base<T, E, false> : expected_copy_base<T, E> {
    using expected_copy_base<T, E>::expected_copy_base;

    expected_move_base() = default;
    expected_move_base(const expected_move_base& rhs) = default;

    expected_move_base(expected_move_base&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        : expected_copy_base<T, E>(no_init) {
        if (rhs.has_value()) {
            this->construct_with(std::move(rhs));
        } else {
            this->construct_error(std::move(rhs.geterr()));
        }
    }
    expected_move_base& operator=(const expected_move_base& rhs) = default;
    expected_move_base& operator=(expected_move_base&& rhs) = default;
};

// This class manages conditionally having a trivial copy assignment operator
template<
    class T,
    class E,
    // clang-format off
         bool = is_void_or_v<T,
                std::conjunction<std::is_trivially_copy_assignable<T>,
                                std::is_trivially_copy_constructible<T>,
                                std::is_trivially_destructible<T>>> &&
             std::is_trivially_copy_assignable_v<E> &&
             std::is_trivially_copy_constructible_v<E> &&
             std::is_trivially_destructible_v<E>
         >
// clang-format on
struct expected_copy_assign_base : expected_move_base<T, E> {
    using expected_move_base<T, E>::expected_move_base;
};

template<class T, class E>
struct expected_copy_assign_base<T, E, false> : expected_move_base<T, E> {
    using expected_move_base<T, E>::expected_move_base;

    expected_copy_assign_base() = default;
    expected_copy_assign_base(const expected_copy_assign_base& rhs) = default;

    expected_copy_assign_base(expected_copy_assign_base&& rhs) = default;
    expected_copy_assign_base& operator=(const expected_copy_assign_base& rhs) {
        this->assign(rhs);
        return *this;
    }
    expected_copy_assign_base& operator=(expected_copy_assign_base&& rhs) = default;
};


// This class manages conditionally having a trivial move assignment operator
// Unfortunately there's no way to achieve this in GCC < 5 AFAIK, since it
// doesn't implement an analogue to std::is_trivially_move_assignable. We have
// to make do with a non-trivial move assignment operator even if T is trivially
// move assignable
template<
    class T,
    class E,
    // clang-format off
    bool = is_void_or_v<T,
        std::conjunction<std::is_trivially_destructible<T>,
                           std::is_trivially_move_constructible<T>,
                           std::is_trivially_move_assignable<T>
                          >> &&
        std::is_trivially_destructible_v<E> &&
        std::is_trivially_move_constructible_v<E> &&
        std::is_trivially_move_assignable_v<E>
    >
// clang-format on
struct expected_move_assign_base : expected_copy_assign_base<T, E> {
    using expected_copy_assign_base<T, E>::expected_copy_assign_base;
};

template<class T, class E>
struct expected_move_assign_base<T, E, false> : expected_copy_assign_base<T, E> {
    using expected_copy_assign_base<T, E>::expected_copy_assign_base;

    expected_move_assign_base() = default;
    expected_move_assign_base(const expected_move_assign_base& rhs) = default;

    expected_move_assign_base(expected_move_assign_base&& rhs) = default;

    expected_move_assign_base& operator=(const expected_move_assign_base& rhs) = default;

    expected_move_assign_base& operator=(expected_move_assign_base&& rhs) noexcept(
        std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>) {
        this->assign(std::move(rhs));
        return *this;
    }
};

// expected_delete_ctor_base will conditionally delete copy and move
// constructors depending on whether T is copy/move constructible
template<class T,
         class E,
         bool EnableCopy = (is_copy_constructible_or_void_v<T> && std::is_copy_constructible_v<E>),
         bool EnableMove = (is_move_constructible_or_void_v<T> && std::is_move_constructible_v<E>)>
struct expected_delete_ctor_base {
    expected_delete_ctor_base() = default;
    expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
    expected_delete_ctor_base& operator=(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base& operator=(expected_delete_ctor_base&&) noexcept = default;
};

template<class T, class E>
struct expected_delete_ctor_base<T, E, true, false> {
    expected_delete_ctor_base() = default;
    expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
    expected_delete_ctor_base& operator=(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base& operator=(expected_delete_ctor_base&&) noexcept = default;
};

template<class T, class E>
struct expected_delete_ctor_base<T, E, false, true> {
    expected_delete_ctor_base() = default;
    expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
    expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
    expected_delete_ctor_base& operator=(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base& operator=(expected_delete_ctor_base&&) noexcept = default;
};

template<class T, class E>
struct expected_delete_ctor_base<T, E, false, false> {
    expected_delete_ctor_base() = default;
    expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
    expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
    expected_delete_ctor_base& operator=(const expected_delete_ctor_base&) = default;
    expected_delete_ctor_base& operator=(expected_delete_ctor_base&&) noexcept = default;
};

// expected_delete_assign_base will conditionally delete copy and move
// constructors depending on whether T and E are copy/move constructible +
// assignable
template<class T,
         class E,
         bool EnableCopy = (is_copy_constructible_or_void_v<T> && std::is_copy_constructible_v<E> &&
                            is_copy_assignable_or_void_v<T> && std::is_copy_assignable_v<E>),
         bool EnableMove = (is_move_constructible_or_void_v<T> && std::is_move_constructible_v<E> &&
                            is_move_assignable_or_void_v<T> && std::is_move_assignable_v<E>)>
struct expected_delete_assign_base {
    expected_delete_assign_base() = default;
    expected_delete_assign_base(const expected_delete_assign_base&) = default;
    expected_delete_assign_base(expected_delete_assign_base&&) noexcept = default;
    expected_delete_assign_base& operator=(const expected_delete_assign_base&) = default;
    expected_delete_assign_base& operator=(expected_delete_assign_base&&) noexcept = default;
};

template<class T, class E>
struct expected_delete_assign_base<T, E, true, false> {
    expected_delete_assign_base() = default;
    expected_delete_assign_base(const expected_delete_assign_base&) = default;
    expected_delete_assign_base(expected_delete_assign_base&&) noexcept = default;
    expected_delete_assign_base& operator=(const expected_delete_assign_base&) = default;
    expected_delete_assign_base& operator=(expected_delete_assign_base&&) noexcept = delete;
};

template<class T, class E>
struct expected_delete_assign_base<T, E, false, true> {
    expected_delete_assign_base() = default;
    expected_delete_assign_base(const expected_delete_assign_base&) = default;
    expected_delete_assign_base(expected_delete_assign_base&&) noexcept = default;
    expected_delete_assign_base& operator=(const expected_delete_assign_base&) = delete;
    expected_delete_assign_base& operator=(expected_delete_assign_base&&) noexcept = default;
};

template<class T, class E>
struct expected_delete_assign_base<T, E, false, false> {
    expected_delete_assign_base() = default;
    expected_delete_assign_base(const expected_delete_assign_base&) = default;
    expected_delete_assign_base(expected_delete_assign_base&&) noexcept = default;
    expected_delete_assign_base& operator=(const expected_delete_assign_base&) = delete;
    expected_delete_assign_base& operator=(expected_delete_assign_base&&) noexcept = delete;
};

// This is needed to be able to construct the expected_default_ctor_base which
// follows, while still conditionally deleting the default constructor.
struct default_constructor_tag {
    explicit constexpr default_constructor_tag() = default;
};

// expected_default_ctor_base will ensure that expected has a deleted default
// consturctor if T is not default constructible.
// This specialization is for when T is default constructible
template<class T, class E, bool Enable = std::is_default_constructible_v<T> || std::is_void_v<T>>
struct expected_default_ctor_base {
    constexpr expected_default_ctor_base() noexcept = default;
    constexpr expected_default_ctor_base(expected_default_ctor_base const&) noexcept = default;
    constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept = default;
    expected_default_ctor_base& operator=(expected_default_ctor_base const&) noexcept = default;
    expected_default_ctor_base& operator=(expected_default_ctor_base&&) noexcept = default;

    constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
};

// This specialization is for when T is not default constructible
template<class T, class E>
struct expected_default_ctor_base<T, E, false> {
    constexpr expected_default_ctor_base() noexcept = delete;
    constexpr expected_default_ctor_base(expected_default_ctor_base const&) noexcept = default;
    constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept = default;
    expected_default_ctor_base& operator=(expected_default_ctor_base const&) noexcept = default;
    expected_default_ctor_base& operator=(expected_default_ctor_base&&) noexcept = default;

    constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
};
} // namespace detail

template<class E>
class bad_expected_access : public std::exception {
    public:
    explicit bad_expected_access(E e) : m_val(std::move(e)) {}

    virtual const char* what() const noexcept override {
        return "Bad expected access";
    }

    const E& error() const& {
        return m_val;
    }
    E& error() & {
        return m_val;
    }
    const E&& error() const&& {
        return std::move(m_val);
    }
    E&& error() && {
        return std::move(m_val);
    }

    private:
    E m_val;
};

/// An `expected<T, E>` object is an object that contains the storage for
/// another object and manages the lifetime of this contained object `T`.
/// Alternatively it could contain the storage for another unexpected object
/// `E`. The contained object may not be initialized after the expected object
/// has been initialized, and may not be destroyed before the expected object
/// has been destroyed. The initialization state of the contained object is
/// tracked by the expected object.
template<class T, class E>
class expected
    : private detail::expected_move_assign_base<T, E>
    , private detail::expected_delete_ctor_base<T, E>
    , private detail::expected_delete_assign_base<T, E>
    , private detail::expected_default_ctor_base<T, E> {
    static_assert(!std::is_reference_v<T>, "T must not be a reference");
    static_assert(!std::is_same_v<T, std::remove_cv<std::in_place_t>>, "T must not be in_place_t");
    static_assert(!std::is_same_v<T, std::remove_cv<unexpect_t>>, "T must not be unexpect_t");
    static_assert(!std::is_same_v<T, std::remove_cv<unexpected<E>>>, "T must not be unexpected<E>");
    static_assert(!std::is_reference_v<E>, "E must not be a reference");

    T* valptr() {
        return std::addressof(this->m_val);
    }
    const T* valptr() const {
        return std::addressof(this->m_val);
    }
    unexpected<E>* errptr() {
        return std::addressof(this->m_unexpect);
    }
    const unexpected<E>* errptr() const {
        return std::addressof(this->m_unexpect);
    }

    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr U& val() {
        return this->m_val;
    }
    constexpr unexpected<E>& err() {
        return this->m_unexpect;
    }

    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr const U& val() const {
        return this->m_val;
    }
    constexpr const unexpected<E>& err() const {
        return this->m_unexpect;
    }

    using impl_base = detail::expected_move_assign_base<T, E>;
    using ctor_base = detail::expected_default_ctor_base<T, E>;

    public:
    typedef T value_type;
    typedef E error_type;
    typedef unexpected<E> unexpected_type;

    template<class F>
    constexpr auto and_then(F&& f) & {
        return and_then_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto and_then(F&& f) && {
        return and_then_impl(std::move(*this), std::forward<F>(f));
    }
    template<class F>
    constexpr auto and_then(F&& f) const& {
        return and_then_impl(*this, std::forward<F>(f));
    }

    template<class F>
    constexpr auto and_then(F&& f) const&& {
        return and_then_impl(std::move(*this), std::forward<F>(f));
    }


    template<class F>
    constexpr auto map(F&& f) & {
        return expected_map_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto map(F&& f) && {
        return expected_map_impl(std::move(*this), std::forward<F>(f));
    }
    template<class F>
    constexpr auto map(F&& f) const& {
        return expected_map_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto map(F&& f) const&& {
        return expected_map_impl(std::move(*this), std::forward<F>(f));
    }

    template<class F>
    constexpr auto transform(F&& f) & {
        return expected_map_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto transform(F&& f) && {
        return expected_map_impl(std::move(*this), std::forward<F>(f));
    }
    template<class F>
    constexpr auto transform(F&& f) const& {
        return expected_map_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto transform(F&& f) const&& {
        return expected_map_impl(std::move(*this), std::forward<F>(f));
    }

    template<class F>
    constexpr auto map_error(F&& f) & {
        return map_error_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto map_error(F&& f) && {
        return map_error_impl(std::move(*this), std::forward<F>(f));
    }
    template<class F>
    constexpr auto map_error(F&& f) const& {
        return map_error_impl(*this, std::forward<F>(f));
    }
    template<class F>
    constexpr auto map_error(F&& f) const&& {
        return map_error_impl(std::move(*this), std::forward<F>(f));
    }

    template<class F>
    expected constexpr or_else(F&& f) & {
        return or_else_impl(*this, std::forward<F>(f));
    }

    template<class F>
    expected constexpr or_else(F&& f) && {
        return or_else_impl(std::move(*this), std::forward<F>(f));
    }

    template<class F>
    expected constexpr or_else(F&& f) const& {
        return or_else_impl(*this, std::forward<F>(f));
    }

    template<class F>
    expected constexpr or_else(F&& f) const&& {
        return or_else_impl(std::move(*this), std::forward<F>(f));
    }
    constexpr expected() = default;
    constexpr expected(const expected& rhs) = default;
    constexpr expected(expected&& rhs) = default;
    expected& operator=(const expected& rhs) = default;
    expected& operator=(expected&& rhs) = default;

    template<class... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>>* = nullptr>
    constexpr expected(std::in_place_t, Args&&... args)
        : impl_base(std::in_place, std::forward<Args>(args)...), ctor_base(detail::default_constructor_tag{}) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr expected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : impl_base(std::in_place, il, std::forward<Args>(args)...), ctor_base(detail::default_constructor_tag{}) {}

    template<class G = E,
             std::enable_if_t<std::is_constructible_v<E, const G&>>* = nullptr,
             std::enable_if_t<!std::is_convertible_v<const G&, E>>* = nullptr>
    explicit constexpr expected(const unexpected<G>& e)
        : impl_base(unexpect, e.value()), ctor_base(detail::default_constructor_tag{}) {}

    template<class G = E,
             std::enable_if_t<std::is_constructible_v<E, const G&>>* = nullptr,
             std::enable_if_t<std::is_convertible_v<const G&, E>>* = nullptr>
    constexpr expected(unexpected<G> const& e)
        : impl_base(unexpect, e.value()), ctor_base(detail::default_constructor_tag{}) {}

    template<class G = E,
             std::enable_if_t<std::is_constructible_v<E, G&&>>* = nullptr,
             std::enable_if_t<!std::is_convertible_v<G&&, E>>* = nullptr>
    explicit constexpr expected(unexpected<G>&& e) noexcept(std::is_nothrow_constructible_v<E, G&&>)
        : impl_base(unexpect, std::move(e.value())), ctor_base(detail::default_constructor_tag{}) {}

    template<class G = E,
             std::enable_if_t<std::is_constructible_v<E, G&&>>* = nullptr,
             std::enable_if_t<std::is_convertible_v<G&&, E>>* = nullptr>
    constexpr expected(unexpected<G>&& e) noexcept(std::is_nothrow_constructible_v<E, G&&>)
        : impl_base(unexpect, std::move(e.value())), ctor_base(detail::default_constructor_tag{}) {}

    template<class... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>>* = nullptr>
    constexpr explicit expected(unexpect_t, Args&&... args)
        : impl_base(unexpect, std::forward<Args>(args)...), ctor_base(detail::default_constructor_tag{}) {}

    template<class U,
             class... Args,
             std::enable_if_t<std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>>* = nullptr>
    constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : impl_base(unexpect, il, std::forward<Args>(args)...), ctor_base(detail::default_constructor_tag{}) {}

    template<class U,
             class G,
             std::enable_if_t<!(std::is_convertible_v<U const&, T> && std::is_convertible_v<G const&, E>)>* = nullptr,
             detail::expected_enable_from_other<T, E, U, G, const U&, const G&>* = nullptr>
    explicit constexpr expected(const expected<U, G>& rhs) : ctor_base(detail::default_constructor_tag{}) {
        if (rhs.has_value()) {
            this->construct(*rhs);
        } else {
            this->construct_error(rhs.error());
        }
    }

    template<class U,
             class G,
             std::enable_if_t<(std::is_convertible_v<U const&, T> && std::is_convertible_v<G const&, E>)>* = nullptr,
             detail::expected_enable_from_other<T, E, U, G, const U&, const G&>* = nullptr>
    constexpr expected(const expected<U, G>& rhs) : ctor_base(detail::default_constructor_tag{}) {
        if (rhs.has_value()) {
            this->construct(*rhs);
        } else {
            this->construct_error(rhs.error());
        }
    }

    template<class U,
             class G,
             std::enable_if_t<!(std::is_convertible_v<U&&, T> && std::is_convertible_v<G&&, E>)>* = nullptr,
             detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
    explicit constexpr expected(expected<U, G>&& rhs) : ctor_base(detail::default_constructor_tag{}) {
        if (rhs.has_value()) {
            this->construct(std::move(*rhs));
        } else {
            this->construct_error(std::move(rhs.error()));
        }
    }

    template<class U,
             class G,
             std::enable_if_t<(std::is_convertible_v<U&&, T> && std::is_convertible_v<G&&, E>)>* = nullptr,
             detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
    constexpr expected(expected<U, G>&& rhs) : ctor_base(detail::default_constructor_tag{}) {
        if (rhs.has_value()) {
            this->construct(std::move(*rhs));
        } else {
            this->construct_error(std::move(rhs.error()));
        }
    }

    template<class U = T,
             std::enable_if_t<!std::is_convertible_v<U&&, T>>* = nullptr,
             detail::expected_enable_forward_value<T, E, U>* = nullptr>
    explicit constexpr expected(U&& v) : expected(std::in_place, std::forward<U>(v)) {}

    template<class U = T,
             std::enable_if_t<std::is_convertible_v<U&&, T>>* = nullptr,
             detail::expected_enable_forward_value<T, E, U>* = nullptr>
    constexpr expected(U&& v) : expected(std::in_place, std::forward<U>(v)) {}

    template<class U = T,
             class G = T,
             std::enable_if_t<std::is_nothrow_constructible_v<T, U&&>>* = nullptr,
             std::enable_if_t<!std::is_void_v<G>>* = nullptr,
             std::enable_if_t<(!std::is_same_v<expected<T, E>, std::decay_t<U>> &&
                               !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
                               std::is_constructible_v<T, U> && std::is_assignable_v<G&, U> &&
                               std::is_nothrow_move_constructible_v<E>)>* = nullptr>
    expected& operator=(U&& v) {
        if (has_value()) {
            val() = std::forward<U>(v);
        } else {
            err().~unexpected<E>();
            ::new (valptr()) T(std::forward<U>(v));
            this->m_has_val = true;
        }

        return *this;
    }

    template<class U = T,
             class G = T,
             std::enable_if_t<!std::is_nothrow_constructible_v<T, U&&>>* = nullptr,
             std::enable_if_t<!std::is_void_v<U>>* = nullptr,
             std::enable_if_t<(!std::is_same_v<expected<T, E>, std::decay_t<U>> &&
                               !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
                               std::is_constructible_v<T, U> && std::is_assignable_v<G&, U> &&
                               std::is_nothrow_move_constructible_v<E>)>* = nullptr>
    expected& operator=(U&& v) {
        if (has_value()) {
            val() = std::forward<U>(v);
        } else {
            auto tmp = std::move(err());
            err().~unexpected<E>();

#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
            try {
                ::new (valptr()) T(std::forward<U>(v));
                this->m_has_val = true;
            } catch (...) {
                err() = std::move(tmp);
                throw;
            }
#else
            ::new (valptr()) T(std::forward<U>(v));
            this->m_has_val = true;
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
        }

        return *this;
    }

    template<class G = E,
             std::enable_if_t<std::is_nothrow_copy_constructible_v<G> && std::is_assignable_v<G&, G>>* = nullptr>
    expected& operator=(const unexpected<G>& rhs) {
        if (!has_value()) {
            err() = rhs;
        } else {
            this->destroy_val();
            ::new (errptr()) unexpected<E>(rhs);
            this->m_has_val = false;
        }

        return *this;
    }

    template<class G = E,
             std::enable_if_t<std::is_nothrow_move_constructible_v<G> && std::is_move_assignable_v<G>>* = nullptr>
    expected& operator=(unexpected<G>&& rhs) noexcept {
        if (!has_value()) {
            err() = std::move(rhs);
        } else {
            this->destroy_val();
            ::new (errptr()) unexpected<E>(std::move(rhs));
            this->m_has_val = false;
        }

        return *this;
    }

    template<class... Args, std::enable_if_t<std::is_nothrow_constructible_v<T, Args&&...>>* = nullptr>
    void emplace(Args&&... args) {
        if (has_value()) {
            val() = T(std::forward<Args>(args)...);
        } else {
            err().~unexpected<E>();
            ::new (valptr()) T(std::forward<Args>(args)...);
            this->m_has_val = true;
        }
    }

    template<class... Args, std::enable_if_t<!std::is_nothrow_constructible_v<T, Args&&...>>* = nullptr>
    void emplace(Args&&... args) {
        if (has_value()) {
            val() = T(std::forward<Args>(args)...);
        } else {
            auto tmp = std::move(err());
            err().~unexpected<E>();

#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
            try {
                ::new (valptr()) T(std::forward<Args>(args)...);
                this->m_has_val = true;
            } catch (...) {
                err() = std::move(tmp);
                throw;
            }
#else
            ::new (valptr()) T(std::forward<Args>(args)...);
            this->m_has_val = true;
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
        }
    }

    template<class U,
             class... Args,
             std::enable_if_t<std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    void emplace(std::initializer_list<U> il, Args&&... args) {
        if (has_value()) {
            T t(il, std::forward<Args>(args)...);
            val() = std::move(t);
        } else {
            err().~unexpected<E>();
            ::new (valptr()) T(il, std::forward<Args>(args)...);
            this->m_has_val = true;
        }
    }

    template<class U,
             class... Args,
             std::enable_if_t<!std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args&&...>>* = nullptr>
    void emplace(std::initializer_list<U> il, Args&&... args) {
        if (has_value()) {
            T t(il, std::forward<Args>(args)...);
            val() = std::move(t);
        } else {
            auto tmp = std::move(err());
            err().~unexpected<E>();

#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
            try {
                ::new (valptr()) T(il, std::forward<Args>(args)...);
                this->m_has_val = true;
            } catch (...) {
                err() = std::move(tmp);
                throw;
            }
#else
            ::new (valptr()) T(il, std::forward<Args>(args)...);
            this->m_has_val = true;
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
        }
    }

    private:
    using t_is_void = std::true_type;
    using t_is_not_void = std::false_type;
    using t_is_nothrow_move_constructible = std::true_type;
    using move_constructing_t_can_throw = std::false_type;
    using e_is_nothrow_move_constructible = std::true_type;
    using move_constructing_e_can_throw = std::false_type;

    void swap_where_both_have_value(expected& /*rhs*/, t_is_void) noexcept {
        // swapping void is a no-op
    }

    void swap_where_both_have_value(expected& rhs, t_is_not_void) {
        using std::swap;
        swap(val(), rhs.val());
    }

    void swap_where_only_one_has_value(expected& rhs, t_is_void) noexcept(std::is_nothrow_move_constructible_v<E>) {
        ::new (errptr()) unexpected_type(std::move(rhs.err()));
        rhs.err().~unexpected_type();
        std::swap(this->m_has_val, rhs.m_has_val);
    }

    void swap_where_only_one_has_value(expected& rhs, t_is_not_void) {
        swap_where_only_one_has_value_and_t_is_not_void(rhs,
                                                        typename std::is_nothrow_move_constructible<T>::type{},
                                                        typename std::is_nothrow_move_constructible<E>::type{});
    }

    void swap_where_only_one_has_value_and_t_is_not_void(expected& rhs,
                                                         t_is_nothrow_move_constructible,
                                                         e_is_nothrow_move_constructible) noexcept {
        auto temp = std::move(val());
        val().~T();
        ::new (errptr()) unexpected_type(std::move(rhs.err()));
        rhs.err().~unexpected_type();
        ::new (rhs.valptr()) T(std::move(temp));
        std::swap(this->m_has_val, rhs.m_has_val);
    }

    void swap_where_only_one_has_value_and_t_is_not_void(expected& rhs,
                                                         t_is_nothrow_move_constructible,
                                                         move_constructing_e_can_throw) {
        auto temp(std::move(val()));
        val().~T();
#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
        try {
            ::new (errptr()) unexpected_type(std::move(rhs.err()));
            rhs.err().~unexpected_type();
            ::new (rhs.valptr()) T(std::move(temp));
            std::swap(this->m_has_val, rhs.m_has_val);
        } catch (...) {
            ::new (valptr()) T(std::move(temp));
            throw;
        }
#else
        ::new (errptr()) unexpected_type(std::move(rhs.err()));
        rhs.err().~unexpected_type();
        ::new (rhs.valptr()) T(std::move(temp));
        std::swap(this->m_has_val, rhs.m_has_val);
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
    }

    void swap_where_only_one_has_value_and_t_is_not_void(expected& rhs,
                                                         move_constructing_t_can_throw,
                                                         t_is_nothrow_move_constructible) {
        auto temp = std::move(rhs.err());
        rhs.err().~unexpected_type();
#ifdef EXT_EXPECTED_EXCEPTIONS_ENABLED
        try {
            ::new (rhs.valptr()) T(val());
            val().~T();
            ::new (errptr()) unexpected_type(std::move(temp));
            std::swap(this->m_has_val, rhs.m_has_val);
        } catch (...) {
            rhs.err() = std::move(temp);
            throw;
        }
#else
        ::new (rhs.valptr()) T(val());
        val().~T();
        ::new (errptr()) unexpected_type(std::move(temp));
        std::swap(this->m_has_val, rhs.m_has_val);
#endif // EXT_EXPECTED_EXCEPTIONS_ENABLED
    }

    public:
    template<class OT = T, class OE = E>
    std::enable_if_t<detail::is_swappable<OT>::value && detail::is_swappable<OE>::value &&
                     (std::is_nothrow_move_constructible_v<OT> || std::is_nothrow_move_constructible_v<OE>)>
        swap(expected& rhs) noexcept(
            std::is_nothrow_move_constructible_v<T>&& detail::is_nothrow_swappable<T>::value&&
                std::is_nothrow_move_constructible_v<E>&& detail::is_nothrow_swappable<E>::value) {
        if (has_value() && rhs.has_value()) {
            swap_where_both_have_value(rhs, typename std::is_void<T>::type{});
        } else if (!has_value() && rhs.has_value()) {
            rhs.swap(*this);
        } else if (has_value()) {
            swap_where_only_one_has_value(rhs, typename std::is_void<T>::type{});
        } else {
            using std::swap;
            swap(err(), rhs.err());
        }
    }

    constexpr const T* operator->() const {
        return valptr();
    }
    constexpr T* operator->() {
        return valptr();
    }

    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr const U& operator*() const& {
        return val();
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr U& operator*() & {
        return val();
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr const U&& operator*() const&& {
        return std::move(val());
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr U&& operator*() && {
        return std::move(val());
    }

    constexpr bool has_value() const noexcept {
        return this->m_has_val;
    }
    constexpr explicit operator bool() const noexcept {
        return this->m_has_val;
    }

    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr const U& value() const& {
        if (!has_value())
            detail::throw_exception(bad_expected_access<E>(err().value()));
        return val();
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr U& value() & {
        if (!has_value())
            detail::throw_exception(bad_expected_access<E>(err().value()));
        return val();
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr const U&& value() const&& {
        if (!has_value())
            detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
        return std::move(val());
    }
    template<class U = T, std::enable_if_t<!std::is_void_v<U>>* = nullptr>
    constexpr U&& value() && {
        if (!has_value())
            detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
        return std::move(val());
    }

    constexpr const E& error() const& {
        return err().value();
    }
    constexpr E& error() & {
        return err().value();
    }
    constexpr const E&& error() const&& {
        return std::move(err().value());
    }
    constexpr E&& error() && {
        return std::move(err().value());
    }

    template<class U>
    constexpr T value_or(U&& v) const& {
        static_assert(std::is_copy_constructible_v<T> && std::is_convertible_v<U&&, T>,
                      "T must be copy-constructible and convertible to from U&&");
        return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
    }
    template<class U>
    constexpr T value_or(U&& v) && {
        static_assert(std::is_move_constructible_v<T> && std::is_convertible_v<U&&, T>,
                      "T must be move-constructible and convertible to from U&&");
        return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
    }
};

namespace detail {
template<class Exp>
using val_t = typename std::decay_t<Exp>::value_type;
template<class Exp>
using err_t = typename std::decay_t<Exp>::error_type;
template<class Exp, class Ret>
using ret_t = expected<Ret, err_t<Exp>>;

template<class Exp,
         class F,
         std::enable_if_t<!std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Exp>()))>
constexpr auto and_then_impl(Exp&& exp, F&& f) {
    static_assert(detail::is_expected_v<Ret>, "F must return an expected");

    return exp.has_value() ? std::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
                           : Ret(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>()))>
constexpr auto and_then_impl(Exp&& exp, F&& f) {
    static_assert(detail::is_expected_v<Ret>, "F must return an expected");

    return exp.has_value() ? std::invoke(std::forward<F>(f)) : Ret(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<!std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Exp>())),
         std::enable_if_t<!std::is_void_v<Ret>>* = nullptr>
constexpr auto expected_map_impl(Exp&& exp, F&& f) {
    using result = ret_t<Exp, std::decay_t<Ret>>;
    return exp.has_value() ? result(std::invoke(std::forward<F>(f), *std::forward<Exp>(exp)))
                           : result(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<!std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Exp>())),
         std::enable_if_t<std::is_void_v<Ret>>* = nullptr>
auto expected_map_impl(Exp&& exp, F&& f) {
    using result = expected<void, err_t<Exp>>;
    if (exp.has_value()) {
        std::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
        return result();
    }

    return result(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>())),
         std::enable_if_t<!std::is_void_v<Ret>>* = nullptr>
constexpr auto expected_map_impl(Exp&& exp, F&& f) {
    using result = ret_t<Exp, std::decay_t<Ret>>;
    return exp.has_value() ? result(std::invoke(std::forward<F>(f))) : result(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>())),
         std::enable_if_t<std::is_void_v<Ret>>* = nullptr>
auto expected_map_impl(Exp&& exp, F&& f) {
    using result = expected<void, err_t<Exp>>;
    if (exp.has_value()) {
        std::invoke(std::forward<F>(f));
        return result();
    }

    return result(unexpect, std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         std::enable_if_t<!std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<!std::is_void_v<Ret>>* = nullptr>
constexpr auto map_error_impl(Exp&& exp, F&& f) {
    using result = expected<val_t<Exp>, std::decay_t<Ret>>;
    return exp.has_value() ? result(*std::forward<Exp>(exp))
                           : result(unexpect, std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error()));
}
template<class Exp,
         class F,
         std::enable_if_t<!std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<std::is_void_v<Ret>>* = nullptr>
auto map_error_impl(Exp&& exp, F&& f) {
    using result = expected<val_t<Exp>, std::monostate>;
    if (exp.has_value()) {
        return result(*std::forward<Exp>(exp));
    }

    std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
    return result(unexpect, std::monostate{});
}
template<class Exp,
         class F,
         std::enable_if_t<std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<!std::is_void_v<Ret>>* = nullptr>
constexpr auto map_error_impl(Exp&& exp, F&& f) {
    using result = expected<val_t<Exp>, std::decay_t<Ret>>;
    return exp.has_value() ? result()
                           : result(unexpect, std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error()));
}
template<class Exp,
         class F,
         std::enable_if_t<std::is_void_v<val_t<Exp>>>* = nullptr,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<std::is_void_v<Ret>>* = nullptr>
auto map_error_impl(Exp&& exp, F&& f) {
    using result = expected<val_t<Exp>, std::monostate>;
    if (exp.has_value()) {
        return result();
    }

    std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
    return result(unexpect, std::monostate{});
}

template<class Exp,
         class F,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<!std::is_void_v<Ret>>* = nullptr>
auto or_else_impl(Exp&& exp, F&& f) -> Ret {
    static_assert(detail::is_expected_v<Ret>, "F must return an expected");
    return exp.has_value() ? std::forward<Exp>(exp) : std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
}

template<class Exp,
         class F,
         class Ret = decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error())),
         std::enable_if_t<std::is_void_v<Ret>>* = nullptr>
std::decay_t<Exp> or_else_impl(Exp&& exp, F&& f) {
    return exp.has_value() ? std::forward<Exp>(exp)
                           : (std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error()), std::forward<Exp>(exp));
}
} // namespace detail

template<class T, class E, class U, class F>
constexpr bool operator==(const expected<T, E>& lhs, const expected<U, F>& rhs) {
    return (lhs.has_value() != rhs.has_value()) ? false
                                                : (!lhs.has_value() ? lhs.error() == rhs.error() : *lhs == *rhs);
}
template<class T, class E, class U, class F>
constexpr bool operator!=(const expected<T, E>& lhs, const expected<U, F>& rhs) {
    return (lhs.has_value() != rhs.has_value()) ? true : (!lhs.has_value() ? lhs.error() != rhs.error() : *lhs != *rhs);
}

template<class T, class E, class U>
constexpr bool operator==(const expected<T, E>& x, const U& v) {
    return x.has_value() ? *x == v : false;
}
template<class T, class E, class U>
constexpr bool operator==(const U& v, const expected<T, E>& x) {
    return x.has_value() ? *x == v : false;
}
template<class T, class E, class U>
constexpr bool operator!=(const expected<T, E>& x, const U& v) {
    return x.has_value() ? *x != v : true;
}
template<class T, class E, class U>
constexpr bool operator!=(const U& v, const expected<T, E>& x) {
    return x.has_value() ? *x != v : true;
}

template<class T, class E>
constexpr bool operator==(const expected<T, E>& x, const unexpected<E>& e) {
    return x.has_value() ? false : x.error() == e.value();
}
template<class T, class E>
constexpr bool operator==(const unexpected<E>& e, const expected<T, E>& x) {
    return x.has_value() ? false : x.error() == e.value();
}
template<class T, class E>
constexpr bool operator!=(const expected<T, E>& x, const unexpected<E>& e) {
    return x.has_value() ? true : x.error() != e.value();
}
template<class T, class E>
constexpr bool operator!=(const unexpected<E>& e, const expected<T, E>& x) {
    return x.has_value() ? true : x.error() != e.value();
}

template<class T,
         class E,
         std::enable_if_t<(std::is_void_v<T> || std::is_move_constructible_v<T>) &&detail::is_swappable<T>::value &&
                          std::is_move_constructible_v<E> && detail::is_swappable<E>::value>* = nullptr>
void swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}
} // namespace ext::util
#endif // EXT_UTIL_EXPECTED_HEADER
