#pragma once
#include <iostream>
#include "logging/log_logger.hpp"

#define OBI_LOG_TOPIC(level_,topic_)                               \
  !obi::util::logging::_detail::level_is_active((level_),(topic_)) \
      ? (void)nullptr                                              \
      : obi::util::logging::_detail::log_stream_consumer{} & (     \
        obi::util::logging::_detail::create_log_stream(            \
                        (topic_),                                  \
                        (level_),                                  \
                        __FILE__,                                  \
                        __LINE__,                                  \
                        __FUNCTION__                               \
                    )                                              \
        )

#define OLOG_TOPIC(level_,topic_) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::topic::topic_))

#define OBI_LOG(level_) \
    OBI_LOG_TOPIC(level_, obi::util::logging::topic::unknown)

#define OLOG(level_) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::topic::unknown))
