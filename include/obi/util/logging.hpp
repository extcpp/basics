#pragma once
#include <iostream>
#include "logging/log_logger.hpp"

#define OBI_LOG_TOPIC(level_,topic)                      \
  !obi::util::logging::logger::is_active((level_)        \
										,(topic)         \
										)                \
      ? (void)nullptr                                    \
      : obi::util::logging::log_stream_consumer{} & (    \
		obi::util::logging::logger::create_log_stream(   \
						(topic),                         \
						(level_),                        \
						__FILE__,                        \
						__LINE__,                        \
						__FUNCTION__                     \
					)                                    \
		)

#define OLOG_TOPIC(level_,topic) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::logger::topic))

#define OBI_LOG(level_) \
    OBI_LOG_TOPIC(level_, obi::util::logging::logger::Default)
#define OLOG(level_) \
    OBI_LOG_TOPIC((obi::util::logging::level::level_) \
                 ,(obi::util::logging::logger::Default))
