// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <cstring>
#include <type_traits>
namespace obi { namespace util {

    template <typename To, typename From>
    std::enable_if_t<!std::is_array<To>::value,void>
    type_cast_unsafe(const From &from, To &to){
        std::memcpy(&to, &from, sizeof(To));
    }

    template <typename To, typename From, std::size_t length>
    std::enable_if_t<std::is_array<To>::value,void>
    type_cast_unsafe(const From &from, To (&to)[length]){
        std::memcpy(&to, &from, sizeof(To)*length);
    }

    //! convert From to To without in a safe way where types are of the same size
    template <typename To, typename From>
    inline To type_cast(const From &from){
        static_assert(sizeof(To) == sizeof(From)
                     ,"type_cast requires types with equal size"
                     );
        To to;
        std::memcpy(&to, &from, sizeof(To));
        return to;
    }

    //! convert From to To without in a safe way where target is bigger
    template <typename To, typename From>
    inline To type_cast_bigger(const From &from){
        // copy `from` size form `from` to `to` and fill rest with 0
        // FFFF      -> 0000 0000 => 0000 FFFF

        static_assert(sizeof(To) >= sizeof(From)
                     ,"type_cast requires target with greater or equal size"
                     );

        To to = To(0);
        std::memcpy(&to, &from, sizeof(From));
        return to;
    }

    //! convert From to To without in a safe way where target is smaller
    template <typename To, typename From>
    inline To type_cast_smaller(const From &from){
        // copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF -> 0000 => FFFF

        static_assert(sizeof(To) <= sizeof(From)
                     ,"type_cast requires target with less or equal size"
                     );

        To to;
        std::memcpy(&to, &from, sizeof(To));
        return to;
    }

    //! convert From to To without in a safe way where tzpes are of different size
    template <typename To, typename From>
    inline To type_cast_diff(const From &from){
        // if `to` is less equal only copy `to` size form `from` to `to`
        // otherwise copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF ->      0000 =>      FFFF
        // FFFF      -> 0000 0000 => 0000 FFFF

        To to = To(0);
        std::memcpy(&to, &from, (sizeof(To) <= sizeof(From)) ? sizeof(To) : sizeof(From));
        return to;
    }


    // enum conversion
	namespace _detail {
        //use underlying type if Type is a enum
		template <typename E>
		using enable_enum_t = std::enable_if_t<std::is_enum<E>::value, std::underlying_type_t<E>>;
	}

	template <typename Enum>
	inline constexpr _detail::enable_enum_t<Enum>
	enum_to_underlying(Enum e) noexcept {
		return static_cast<std::underlying_type_t<Enum>>(e);
	}

	template <typename Enum, typename T>
	inline constexpr std::enable_if_t<std::is_enum<Enum>::value && std::is_integral<T>::value, Enum>
	underlying_to_enum(T value) noexcept {
		return static_cast<Enum>(value);
	}

	template <typename EnumOut, typename EnumIn>
	inline constexpr std::enable_if_t<std::is_enum<EnumIn>::value && std::is_enum<EnumOut>::value, EnumOut>
	enum_to_enum_unsafe(EnumIn e) noexcept {
		return underlying_to_enum<EnumOut>(enum_to_underlying(e));
	}

	template <typename EnumOut, typename EnumIn>
	inline constexpr std::enable_if_t<std::is_enum<EnumIn>::value &&
                                      std::is_enum<EnumOut>::value &&
                                      std::is_same<std::underlying_type_t<EnumIn>,std::underlying_type_t<EnumOut>>::value, EnumOut>
	enum_to_enum(EnumIn e) noexcept {
		return underlying_to_enum<EnumOut>(enum_to_underlying(e));
	}

}} // obi::util
