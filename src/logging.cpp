#include <ext/logging.hpp>
#include <ext/macros/compiler.hpp>
#include <ext/util/except.hpp>

namespace ext { namespace logging {

/////////////////////////////////////////////////////////////////////////////
EXT_INIT_PRIORITY_VC_HIGH
// NEEDS TO BE CREATED FIRST!!!!
EXT_INIT_PRIORITY_GNU(101) std::mutex _detail::logmutex{};

// on construction topics register in this map beware of fiasco
EXT_INIT_PRIORITY_GNU(102)
std::map<int, _detail::logtopic*> _detail::topics_map{};

EXT_INIT_PRIORITY_GNU(103)
_detail::logtopic topic::no_topic{1, std::string("default"), level::info};
EXT_INIT_PRIORITY_GNU(103)
_detail::logtopic topic::dev{2, std::string("development"), level::debug};
EXT_INIT_PRIORITY_GNU(103)
_detail::logtopic topic::network{3, std::string("network"), level::error};
EXT_INIT_PRIORITY_GNU(103)
_detail::logtopic topic::engine{4, std::string("engine"), level::error};

bool configuration::prefix_newline{false};
bool configuration::append_newline{true};
bool configuration::threads{false};
bool configuration::filename{true};
bool configuration::function{true};
bool configuration::vim{false};
bool configuration::gdb{false};

EXT_INIT_PRIORITY_VC_LOW
/////////////////////////////////////////////////////////////////////////////

// the logger is a class that creates the log stream and writes
// it thread safe to a file descriptor
_detail::logger::logger(
    char const* id, logtopic const& topic, level level_, const char* file_name, int line_no, const char* function)
    : _ss(), _out(std::cerr) {
    _level = level_;
    if (configuration::prefix_newline) {
        _ss << "\n";
    }
    // # vim <filename> +<lineno>
    if (configuration::vim) {
        _ss << "# vim " << file_name << " +" << line_no << "\n";
    }

    if (configuration::gdb) {
#ifndef _WIN32
        _ss << "# break " << ext::util::filename(file_name) << ":" << line_no << "\n";
#endif
    }

    if (true) {
        _ss << "[" << id << "] ";
    }
    // log level
    _ss << level_to_str(level_);

    // log topic
    if (topic.id != topic::no_topic.id) {
        _ss << " (" << topic.name << ")";
    }

    // log filename
    if (configuration::filename) {
        if (!configuration::gdb) {
            _ss << " "
#ifndef _WIN32
                << ext::util::filename(file_name)
#else
                << file_name
#endif
                << ":" << line_no;
        }
    }

    // log function name
    if (configuration::function) {
        _ss << " in " << function << "()";
    }
    _ss << ": '";
}

void _detail::logger::write() {
    std::lock_guard<std::mutex> lock(logmutex);

    _out << _ss.rdbuf() << "'";
    if (configuration::append_newline) {
        _out << "\n";
    }
    _out << std::flush; // close message

    if (_level == level::fatal) {
        std::terminate();
    }
}

_detail::logger::~logger() {
    try {
        write();
    } catch (...) {}
}
}} // namespace ext::logging
