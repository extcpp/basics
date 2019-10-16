// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_LOGGING_DEFINITIONS_HEADER
#define EXT_LOGGING_DEFINITIONS_HEADER

#ifndef EXT_LOGGING_DEFAULT_LEVEL
    #define EXT_LOGGING_DEFAULT_LEVEL info
#endif // EXT_LOGGING_DEFAULT_LEVEL

#include <ext/macros/compiler.hpp>
#include <map>
#include <mutex>
#include <string>

namespace ext { namespace logging {
namespace _detail {
struct logtopic;
EXT_EXPORT_VC extern std::mutex logmutex;
EXT_EXPORT_VC extern std::map<int, logtopic*> topics_map;
} // namespace _detail

enum class level : int { fatal = 0, error = 20, warn = 40, info = 60, debug = 80, trace = 100 };

namespace _detail {
inline std::string const level_to_str(level level_) {
    using namespace std::literals::string_literals;
    switch (level_) {
        case level::fatal:
            return "fatal"s;
        case level::error:
            return "error"s;
        case level::warn:
            return "warning"s;
        case level::info:
            return "info"s;
        case level::debug:
            return "debug"s;
        case level::trace:
            return "trace"s;
        default:
            return "unknown"s;
    }
}

struct logtopic {
    logtopic(int id_, std::string&& name_, level activation_level_)
        : id(id_), activation_level(activation_level_), name(std::move(name_)) {
        topics_map[id] = this;
    }

    logtopic(logtopic&& other) = default;
    logtopic(logtopic const& other) = default;

    int id;
    static const level default_level = level::EXT_LOGGING_DEFAULT_LEVEL;
    // using info is the default
    level activation_level;
    std::string name;
};

} // namespace _detail

namespace configuration {
extern std::map<int, _detail::logtopic*> topics;
// logging is configured globally via these variables
// configure logging before you start logging!!!
EXT_EXPORT_VC extern bool prefix_newline;
EXT_EXPORT_VC extern bool append_newline;
EXT_EXPORT_VC extern bool prefix_newline;
EXT_EXPORT_VC extern bool append_newline;
EXT_EXPORT_VC extern bool threads;
EXT_EXPORT_VC extern bool filename;
EXT_EXPORT_VC extern bool function;
EXT_EXPORT_VC extern bool vim;
EXT_EXPORT_VC extern bool gdb;
} // namespace configuration

namespace topic {
// can not be inline as they shall be mutable
EXT_EXPORT_VC extern _detail::logtopic no_topic;
EXT_EXPORT_VC extern _detail::logtopic dev;
EXT_EXPORT_VC extern _detail::logtopic network;
EXT_EXPORT_VC extern _detail::logtopic engine;
} // namespace topic
}}     // namespace ext::logging
#endif // EXT_LOGGING_DEFINITIONS_HEADER
