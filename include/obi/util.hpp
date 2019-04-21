// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once
#ifndef OBI_UTIL_HEADER
#define OBI_UTIL_HEADER

#include "util/scoped_timer.hpp"
#include "util/type_to_string.hpp"
#include "util/function_cache.hpp"
#include "util/tracing_classes.hpp"
#include "util/container_helper.hpp"

#if __cpp_generic_lambdas >= 201304
    #include "util/functional.hpp"
#endif
#endif // OBI_UTIL_HEADER