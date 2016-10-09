#pragma once
#include <iostream>
#include "logging/log_logger.hpp"

#define OBI_TOPICLOG(topic,level)                                                   \
  !obi::util::logging::logger::is_active((obi::util::logging::log_level::level)\
										,(topic)                               \
										)                                      \
      ? (void)nullptr                                                          \
      : stream_to_void{} & (                                                   \
		std::cerr << obi::util::logging::logger::create_log_stream(            \
						(topic),                                               \
						(obi::util::logging::log_level::level),                \
						__FILE__,                                              \
						__LINE__,                                              \
						__FUNCTION__                                           \
					)                                                          \
		)

#define OBI_LOG(level) OBI_TOPICLOG(obi::util::logging::logger::Default,level)
