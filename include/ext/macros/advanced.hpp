// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef EXT_MACROS_ADVANCED_HEADER
#define EXT_MACROS_ADVANCED_HEADER

#include <ext/macros/general.hpp>

// check for marker /////////////////////////////////////////////////
#define EXT_GET2ND(first, second, ...) second

// returns 0 is there is only one arg. return 2 arg otherwise
#define _EXT_ZERO_IF_ONE_ARG(...) EXT_GET2ND(__VA_ARGS__, 0,)
#define _EXT_SET2ND_TO_ONE(first) first, 1

// Now we can test for the marker if it gets expanded
// the 2nd arg should be set to 1 and that shall be returned
#define EXT_TEST _EXT_ZERO_IF_ONE_ARG
#define EXT_MARK _EXT_SET2ND_TO_ONE
// check for marker - end ///////////////////////////////////////////

// not required since c++17
namespace ext { namespace macros { namespace _detail{
        using expand_type = int[];
}}}
#define EXT_EXPAND_SIDE_EFFECTS(expression) int pass[]{ ((expression), 0)... }; (void)pass;


#endif // EXT_MACROS_ADVANCED_HEADER
