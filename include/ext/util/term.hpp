// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
// requires c++17 (string_view)
#ifndef EXT_UTIL_TERM_HEADER
#define EXT_UTIL_TERM_HEADER

#include <optional>
#include <utility>
#ifdef EXT_USE_TERM
    #include <ext/macros/platform.hpp>
    #ifdef EXT_UNIX
        #if __has_include(<sys/ioctl.h>) && __has_include(<unistd.h>)
            #include <sys/ioctl.h>
            #include <unistd.h>
            #define EXT_UNIX_TERM
        #endif
    #elif EXT_WINDOWS
        #include <windows.h>
        #define EXT_WINDOWS_TERM
    #endif
#endif // EXT_USE_TERM

namespace ext { namespace util {

inline std::optional<std::pair<std::size_t, std::size_t>> get_term_size() {
#ifdef EXT_UNIX_TERM
    struct ::winsize size;
    ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return std::pair<std::size_t, std::size_t>{size.ws_col, size.ws_row};
#elif EXT_WINDOWS_TERM
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    std::size_t columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    std::size_t rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {columns, row};
#else
    return std::nullopt;
#endif // EXT_UNIX_TERM
}

}} // namespace ext::util


#endif // EXT_UTIL_TERM_HEADER
