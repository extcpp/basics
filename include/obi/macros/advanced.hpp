// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MACROS_ADVANCED_HEADER
#define OBI_MACROS_ADVANCED_HEADER

#include <obi/macros/general.hpp>

#define OBI_DELETE(...)
#define OBI_EXPAND(...) __VA_ARGS__

// check for marker /////////////////////////////////////////////////
#define OBI_GET2ND(first, second, ...) second

// returns 0 is there is only one arg. return 2 arg otherwise
#define _OBI_ZERO_IF_ONE_ARG(...) OBI_GET2ND(__VA_ARGS__, 0,)
#define _OBI_SET2ND_TO_ONE(first) first, 1

// Now we can test for the marker if it gets expanded
// the 2nd arg should be set to 1 and that shall be returned
#define OBI_TEST _OBI_ZERO_IF_ONE_ARG
#define OBI_MARK _OBI_SET2ND_TO_ONE
// check for marker - end ///////////////////////////////////////////

// not required since c++17
namespace obi { namespace macros { namespace _detail{
        using expand_type = int[];
}}}
#define OBI_EXPAND_SIDE_EFFECTS(expression) int pass[]{ ((expression), 0)... }; (void)pass;


#endif // OBI_MACROS_ADVANCED_HEADER
