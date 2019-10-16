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
//  EXT_LOG("cafe", info) << "hi there";
//  EXT_LOG("babe", network, error) << "your network is broken";
//  EXT_LOG("2bad", fatal) << "your app will terminate";

#ifndef EXT_LOGGING_HEADER
#define EXT_LOGGING_HEADER
#include <ext/logging/functionality.hpp>
#include <ext/macros/compiler.hpp>
#include <iostream>
#include <type_traits>

// If use_default is true the check becomes constexpr. Therefore the compiler
// can optimize the branch containing the logging instructions in case of
// inactivity completely away. If you want the level to be configurable at
// runtime you need to pay for it by having the instructions in your code.

// TODO -- static_assert parameters
#define _EXT_LOG_INTERNAL(id_, use_default_, topic_, macro_level_, cond_)                  \
    !((use_default_ ? ext::logging::_detail::default_level_is_active((macro_level_))       \
                    : ext::logging::_detail::level_is_active((macro_level_), (topic_))) && \
      (cond_))                                                                             \
        ? (void) nullptr                                                                   \
        : ext::logging::_detail::logger(id_, (topic_), (macro_level_), __FILE__, __LINE__, __FUNCTION__)

#define _EXT_LOG_INTERNAL_ADD_PREFIX(id_, use_default_, topic_, macro_level_, cond_) \
    _EXT_LOG_INTERNAL(id_, use_default_, (ext::logging::topic::topic_), (ext::logging::level::macro_level_), cond_)

#define _EXT_LOG_SELECT5TH_PARAMETER(_1, _2, _3, _4, NAME, ...) NAME

// constexpr macros
#define EXT_LOGC4(id, topic_, macro_level_, cond_) _EXT_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, macro_level_, cond_)

#define EXT_LOGC3(id, topic_, macro_level_) _EXT_LOG_INTERNAL_ADD_PREFIX(id, true, topic_, macro_level_, true)

#define EXT_LOGC2(id, macro_level_) _EXT_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, macro_level_, true)

#define EXT_LOGC1(id) _EXT_LOG_INTERNAL_ADD_PREFIX(id, true, no_topic, EXT_LOGGING_DEFAULT_LEVEL, true)

#define EXT_DEVC _EXT_LOG_INTERNAL_ADD_PREFIX("@@@@", true, dev, EXT_LOGGING_DEFAULT_LEVEL, true)

// 1st __VA_ARGS__ shifts the args into the correct position
// macro can not be empty because of the leading , (fix with __VA_OPT__ in
// c++20)
#ifdef EXT_COMPILER_VC
    // __VA_ARGS__ does not expand on windows :(
    #define _EXT_LOG_EXPAND(x) x
    #define EXT_LOGC(...)                                                                                        \
        _EXT_LOG_SELECT5TH_PARAMETER(_EXT_LOG_EXPAND(__VA_ARGS__), EXT_LOGC4, EXT_LOGC3, EXT_LOGC2, EXT_LOGC1, ) \
        (_EXT_LOG_EXPAND(__VA_ARGS__))
#else
    #define EXT_LOGC(...)                                                                       \
        _EXT_LOG_SELECT5TH_PARAMETER(__VA_ARGS__, EXT_LOGC4, EXT_LOGC3, EXT_LOGC2, EXT_LOGC1, ) \
        (__VA_ARGS__)
#endif // EXT_COMPILER_VC

// runtime configurable macros
#define EXT_LOGV4(id, topic_, macro_level_, cond_) \
    _EXT_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, macro_level_, cond_)

#define EXT_LOGV3(id, topic_, macro_level_) _EXT_LOG_INTERNAL_ADD_PREFIX(id, false, topic_, macro_level_, true)

#define EXT_LOGV2(id, macro_level_) _EXT_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, macro_level_, true)

#define EXT_LOGV1(id) _EXT_LOG_INTERNAL_ADD_PREFIX(id, false, no_topic, EXT_LOGGING_DEFAULT_LEVEL, true)

#define EXT_DEVV _EXT_LOG_INTERNAL_ADD_PREFIX("$$$$", false, dev, EXT_LOGGING_DEFAULT_LEVEL, true)

#define EXT_LOGV(...)                                                                       \
    _EXT_LOG_SELECT5TH_PARAMETER(__VA_ARGS__, EXT_LOGV4, EXT_LOGV3, EXT_LOGV2, EXT_LOGV1, ) \
    (__VA_ARGS__)

// set default macros
#define EXT_LOG EXT_LOGC
#define EXT_DEV EXT_DEVC

#endif // EXT_LOGGING_HEADER
