// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once

#include <type_traits>
namespace obi { namespace meta {

// if_all  - logical and (fold expression)
template<bool ...XS>
using if_all = bool_constant<(XS && ...)>;

// if_any - logical or (fold expression
template<bool ...XS>
using if_any = bool_constant< !if_all<!XS...>::value // share constructed types
                            >;

template<typename T, bool ...XS>
using enable_if_all = std::enable_if<(XS && ...),T>;

template<typename T, bool ...XS>
using enable_if_any = std::enable_if<(XS || ...),T>;


}}  // namespace obi::meta
