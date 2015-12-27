// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <cstring>
namespace obi { namespace util {

    //! convert From to To without in a safe way where types are of the same size
    template <typename To, typename From>
    inline To type_cast(const From &from){
        static_assert(sizeof(T) == sizeof(From)
                     ,"type_cast requires types with equal size"
                     );
        To to;
        std::memcpy(&to, &from, sizeof(T));
        return to;
    }

    //! convert From to To without in a safe way where target is bigger
    template <typename To, typename From>
    inline To type_cast_bigger(const From &from){
    {
        // copy `from` size form `from` to `to` and fill rest with 0
        // FFFF      -> 000 000 => 0000 FFFF

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
    {
        // copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF -> 000 => FFFF

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
    {
        // if `to` is less equal only copy `to` size form `from` to `to`
        // otherwise copy `from` size form `from` to `to` and fill rest with 0
        // 0000 FFFF -> 000     =>      FFFF
        // FFFF      -> 000 000 => 0000 FFFF

        To to = To(0);
        std::memcpy(&to, &from, (sizeof(To) <= sizeof(From)) ? sizeof(To) : sizeof(From));
        return to;
    }

}} // obi::util
