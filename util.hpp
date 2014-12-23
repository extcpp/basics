// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only

#include "util/scoped_timer.hpp"
#include "util/type_to_string.hpp"
#include "util/function_cache.hpp"

#if __cpp_generic_lambdas >= 201304
    #include "util/functional.hpp"
#endif
