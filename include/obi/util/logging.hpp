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
//  OBI_LOG("cafe", info) << "hi there";
//  OBI_LOG("babe", network, error) << "your network is broken";
//  OBI_LOG("2bad", fatal) << "your app will terminate";

#pragma once
#ifndef OBI_UTIL_LOGGING_HEADER
#define OBI_UTIL_LOGGING_HEADER
#include <iostream>
#include <type_traits>
#include <obi/macros/general.hpp>
#include <obi/util/logging/functionality.hpp>

// If use_default is true the check becomes constexpr. Therefore the compiler
// can optimize the branch containing the logging instructions in case of
// inactivity completely away. If you want the level to be configurable at
// runtime you need to pay for it by having the instructions in your code.

// TODO -- static_assert parameters
#define _OBI_LOG_INTERNAL(id_, use_default_, topic_, macro_level_, cond_)                    \
    !((use_default_ ? obi::util::logging::_detail::default_level_is_active((macro_level_))   \
                    : obi::util::logging::_detail::level_is_active((macro_level_), (topic_)) \
    ) && (cond_))                                                                            \
    ? (void)nullptr : obi::util::logging::_detail::logger(                                   \
                        id_, (topic_), (macro_level_),__FILE__, __LINE__, __FUNCTION__ )


#define _OBI_LOG_INTERNAL_ADD_PREFIX(id_, use_default_, topic_, macro_level_, cond_)         \
    _OBI_LOG_INTERNAL(id_, use_default_                                                      \
                     ,(obi::util::logging::topic::topic_)                                    \
                     ,(obi::util::logging::level::macro_level_)                              \
                     , cond_)


#define _OBI_LOG_SELECT5TH_PARAMETER(_1, _2, _3, _4, NAME, ...) NAME

// constexpr macros
#define OBI_LOGC4(id, topic_, macro_level_, cond_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, macro_level_, cond_)

#define OBI_LOGC3(id, topic_, macro_level_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, true, topic_, macro_level_, true)

#define OBI_LOGC2(id, macro_level_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, macro_level_, true)

#define OBI_LOGC1(id) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, OBI_LOGGING_DEFAULT_LEVEL, true)

#define OBI_DEVC \
    _OBI_LOG_INTERNAL_ADD_PREFIX("@@@@", true, dev, OBI_LOGGING_DEFAULT_LEVEL, true)


// 1st __VA_ARGS__ shifts the args into the correct position
// macro can not be empty because of the leading , (fix with __VA_OPT__ in c++20)
#define OBI_LOGC(...) _OBI_LOG_SELECT5TH_PARAMETER(__VA_ARGS__,OBI_LOGC4, OBI_LOGC3, OBI_LOGC2, OBI_LOGC1,)(__VA_ARGS__)


// runtime configurable macros
#define OBI_LOGV4(id, topic_, macro_level_, cond_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, macro_level_, cond_)

#define OBI_LOGV3(id, topic_, macro_level_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, false, topic_, macro_level_, true)

#define OBI_LOGV2(id, macro_level_) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, macro_level_, true)

#define OBI_LOGV1(id) \
    _OBI_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, OBI_LOGGING_DEFAULT_LEVEL, true)

#define OBI_DEVV \
    _OBI_LOG_INTERNAL_ADD_PREFIX("$$$$", false, dev, OBI_LOGGING_DEFAULT_LEVEL, true)

#define OBI_LOGV(...) _OBI_LOG_SELECT5TH_PARAMETER(__VA_ARGS__,OBI_LOGV4, OBI_LOGV3, OBI_LOGV2, OBI_LOGV1,)(__VA_ARGS__)

// set default macros
#define OBI_LOG OBI_LOGC
#define OBI_DEV OBI_DEVC


#endif // OBI_UTIL_LOGGING_HEADER
