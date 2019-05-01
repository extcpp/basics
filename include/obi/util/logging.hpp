// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

// Logging:
//
// There are topics and levels. Topics provide names and contain some extra
// information. Levels range from 0 (fatal) to 100 (trace) and are used when
// writing a log line.
//
// Furthermore topics contain an internal activation level. When a level is
// passed into the macro below then this `macro_level_` is checked  against the
// activation level. If the `topic::activation_level` is greater or equal than
// the `macro_level_`, then a log message is emitted.
//
// Logtopics, a mutex and a topic_map are automatically created using compiler
// specific functionality in order to avoid the static initialisation fiasco.
//
//
// Usage
//  OBI_LOG(info) << "hi there";
//  OBI_LOG(network, error) << "your network is broken";
//  OBI_LOG(fatal) << "your app will terminate";

#pragma once
#ifndef OBI_UTIL_LOGGING_HEADER
#define OBI_UTIL_LOGGING_HEADER
#include <iostream>
#include <obi/util/logging/functionality.hpp>

#define OBI_LOG_TOPIC_IF(topic_, macro_level_, cond)  \
    !obi::util::logging::_detail::level_is_active(    \
        (obi::util::logging::level::macro_level_),    \
        (obi::util::logging::topic::topic_)) && (cond)\
        ? (void)nullptr                               \
        : obi::util::logging::_detail::logger(        \
                (obi::util::logging::topic::topic_),  \
                (obi::util::logging::level::macro_level_),  \
                __FILE__,                             \
                __LINE__,                             \
                __FUNCTION__                          \
            )                                         \

#define OBI_LOG_TOPIC(topic_, macro_level_) \
    OBI_LOG_TOPIC_IF(no_topic, macro_level_, true)

#define OBI_LOG(macro_level_) \
    OBI_LOG_TOPIC(no_topic, macro_level_)

#define OBI_DEV(macro_level_) \
    OBI_LOG_TOPIC(no_topic, macro_level_)

#endif // OBI_UTIL_LOGGING_HEADER
