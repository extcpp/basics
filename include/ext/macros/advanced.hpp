// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#ifndef EXT_MACROS_ADVANCED_HEADER
#define EXT_MACROS_ADVANCED_HEADER

#include <ext/macros/general.hpp>

// check for marker /////////////////////////////////////////////////
#define EXT_GET2ND(first, second, ...) second

// returns 0 if there is only one arg. return 2 arg otherwise
#define EXT_INTERNAL_ZERO_IF_ONE_ARG(...) EXT_GET2ND(__VA_ARGS__, 0, )
#define EXT_INTERNAL_SET2ND_TO_ONE(first) first, 1

// Now we can test for the marker if it gets expanded
// the 2nd arg should be set to 1 and that shall be returned
#define EXT_TEST EXT_INTERNAL_ZERO_IF_ONE_ARG
#define EXT_MARK EXT_INTERNAL_SET2ND_TO_ONE
// check for marker - end ///////////////////////////////////////////

#endif // EXT_MACROS_ADVANCED_HEADER
