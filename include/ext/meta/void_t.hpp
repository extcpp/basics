// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_META_VOID_T_HEADER
#define EXT_META_VOID_T_HEADER
namespace ext { namespace meta {

// void_t
    namespace _detail {
        template<class ...>
        struct _void_t {
            using type = void;
        };
    }

    //alias
    template<class ...XXS>
    using void_t = typename _detail::_void_t<XXS...>::type;

}}  // namespace ext::meta
#endif // EXT_META_VOID_T_HEADER
