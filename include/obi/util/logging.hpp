// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_LOGGING_HEADER
#define OBI_UTIL_LOGGING_HEADER
#include <iostream>
#include <obi/util/logging/functionality.hpp>

#define OBI_LOG_TOPIC(level_,topic_)                  \
    !obi::util::logging::_detail::level_is_active(    \
        (obi::util::logging::level::level_),          \
        (obi::util::logging::topic::topic_))          \
        ? (void)nullptr                               \
        : obi::util::logging::_detail::logger(        \
                (obi::util::logging::topic::topic_),  \
                (obi::util::logging::level::level_),  \
                __FILE__,                             \
                __LINE__,                             \
                __FUNCTION__                          \
            )                                         \

#define OBI_LOG(level_) \
    OBI_LOG_TOPIC(level_,no_topic)
#endif // OBI_UTIL_LOGGING_HEADER