// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <cstring>
#include <type_traits>
#include <limits>
#include <stdexcept>

namespace obi { namespace util {

//// singed <-> unsingend - conversion

template <typename T, typename S = std::make_unsigned_t<T>> constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, S>
to_unsigned(T in){
    return S(in);
}

template <typename T, typename S = std::make_signed_t<T>> constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, S>
to_signed(T in){
    return S(in);
}

template <typename T, typename S = std::make_unsigned_t<T>> constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, S>
to_unsigned_checked(T in){
    if (in < T(std::numeric_limits<S>::min())) {
        throw std::logic_error("conversion to_unsigend not possible");
    }
    return S(in);
}

template <typename T, typename S = std::make_signed_t<T>> constexpr
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, S>
to_signed_checked(T in){
    if (in > T(std::numeric_limits<S>::max)) {
        throw std::logic_error("conversion to_sigend not possible");
    }
    return S(in);
}


    template <typename To, typename From>
    std::enable_if_t<!std::is_array<To>::value,void>
    convert(const From &from, To &to){
        std::memcpy(&to, &from, sizeof(To));
    }

    template <typename To, typename From, std::size_t length>
    std::enable_if_t<std::is_array<To>::value,void>
    convert(const From &from, To (&to)[length]){
        std::memcpy(&to, &from, sizeof(To)*length);
    }

    template <typename To, typename From>
    inline To convert_checked(const From &from){
        static_assert(sizeof(To) == sizeof(From)
                     ,"type_cast requires types with equal size"
                     );
        To to;
        std::memcpy(&to, &from, sizeof(To));
        return to;
    }

    template <typename To, typename From>
    inline To convert_to_bigger(const From &from){
        // copy `from` size form `from` to `to` and fill rest with 0
        // FFFF      -> 0000 0000 => 0000 FFFF

        static_assert(sizeof(To) >= sizeof(From)
                     ,"type_cast requires target with greater or equal size"
                     );

        To to = To(0);
        std::memcpy(&to, &from, sizeof(From));
        return to;
    }

    template <typename To, typename From>
    inline To convert_to_smaller(const From &from){
        // copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF -> 0000 => FFFF

        static_assert(sizeof(To) <= sizeof(From)
                     ,"type_cast requires target with less or equal size"
                     );

        To to;
        std::memcpy(&to, &from, sizeof(To));
        return to;
    }

    template <typename To, typename From>
    inline To convert_different(const From &from){
        // if `to` is less equal only copy `to` size form `from` to `to`
        // otherwise copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF ->      0000 =>      FFFF
        // FFFF      -> 0000 0000 => 0000 FFFF

        To to = To(0);
        std::memcpy(&to, &from, (sizeof(To) <= sizeof(From)) ? sizeof(To) : sizeof(From));
        return to;
    }

    bool d = std::is_enum_v<int>;
    // enum conversion
	namespace _detail {
        //use underlying type if Type is a enum
		template <typename E, typename U = std::underlying_type_t<E>>
		using enable_if_enum_t = std::enable_if_t<std::is_enum_v<E>, U>;

		template <typename E1, typename E2>
		constexpr bool is_enums_v = std::is_enum_v<E1> && std::is_enum_v<E2>;

		template <typename E1, typename E2>
        constexpr bool is_same_underlying_v = std::is_same_v<std::underlying_type_t<E1>,std::underlying_type_t<E1>>;
	}


	template <typename T, typename Enum, typename U = std::underlying_type_t<Enum>>
    inline constexpr std::enable_if_t<std::is_enum_v<Enum> && std::is_same_v<T,U>>
	enum_to_underlying(Enum e) noexcept {
		return static_cast<U>(e);
	}

    template <typename Enum, typename U = std::underlying_type_t<Enum>>
    inline constexpr std::enable_if_t<std::is_enum_v<Enum>>
	enum_to_underlying_unsafe(Enum e) noexcept {
		return static_cast<U>(e);
	}

	template <typename Enum, typename T, typename U = std::underlying_type_t<Enum>>
	inline constexpr std::enable_if_t<std::is_enum_v<Enum> && std::is_same_v<U,T>, Enum>
	underlying_to_enum(T value) noexcept {
		return static_cast<Enum>(value);
	}

	template <typename EnumOut, typename EnumIn>
	inline constexpr std::enable_if_t<_detail::is_enums_v<EnumIn, EnumOut>, EnumOut>
	enum_to_enum_unsafe(EnumIn e) noexcept {
		return underlying_to_enum<EnumOut>(enum_to_underlying(e));
	}

	template <typename EnumOut, typename EnumIn>
	inline constexpr std::enable_if_t<_detail::is_enums_v<EnumIn, EnumOut> &&
                                      _detail::is_same_underlying_v<EnumIn,EnumOut>, EnumOut>
	enum_to_enum(EnumIn e) noexcept {
		return underlying_to_enum<EnumOut>(enum_to_underlying(e));
	}

}} // obi::util
