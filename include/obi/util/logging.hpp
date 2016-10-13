#pragma once
#include <iostream>
#include <obi/util/logging/functionality.hpp>

#define OBI_LOG_TOPIC(level_,topic_)                               \
  !obi::util::logging::_detail::level_is_active((level_),(topic_)) \
      ? (void)nullptr                                              \
      : obi::util::logging::_detail::logger(                       \
                        (topic_),                                  \
                        (level_),                                  \
                        __FILE__,                                  \
                        __LINE__,                                  \
                        __FUNCTION__                               \
                    )                                              \

#define OLOG_TOPIC(level_,topic_) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::topic::topic_))

#define OBI_LOG(level_) \
    OBI_LOG_TOPIC(level_, obi::util::logging::topic::no_topic)

#define OLOG(level_) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::topic::no_topic))
