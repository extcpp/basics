// Copyright - 2015-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

// Collection of functions that convert between utf8 encoded
// strings (std::string and char*) and strings taken by the Windows
// API (std::wstring wchar_t*).
// For more information please refer to:
//
// - https://docs.microsoft.com/en-us/windows/desktop/api/stringapiset/nf-stringapiset-widechartomultibyte
// - https://docs.microsoft.com/en-us/windows/desktop/api/stringapiset/nf-stringapiset-multibytetowidechar
//
//
//          | non UNICODE          | UNICODE
//  --------|----------------------|-----------------------
//  LPCTSTR | LPCSTR (const char*) | LPCWSTR (const wchar*)
//  LPTSTR  | LPSTR (char*)        | LPWSTR (wchar*)
//


#pragma once
#ifndef EXT_UTIL_WINDOWS_STRINGS_HEADER
#define EXT_UTIL_WINDOWS_STRINGS_HEADER

#include <ext/macros/platform.hpp>
#include <ext/util/except.hpp>
#include <string>
#include <memory>

#ifdef EXT_WINDOWS
#include <windows.h>

namespace ext { namespace util {
static_assert(std::is_same_v<LPCSTR, char const*>);
static_assert(std::is_same_v<LPSTR, char*>);
static_assert(std::is_same_v<LPCWSTR, wchar_t const*>);
static_assert(std::is_same_v<LPWSTR, wchar_t*>);


inline std::string win_error_to_string(DWORD error_number) {
    using namespace std::literals::string_literals;

    if(error_number == 0) {
        return ""s; //No error message has been recorded
	}

    LPSTR message_buffer = nullptr;
    size_t size = FormatMessageA(
					  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // dwFlags
				      nullptr,                                    // lpSource
				      error_number,                               // dwMessageId
				      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // dwLanguageId
				      (LPSTR)&message_buffer,                     // lpBuffer
				      0,                                          // nSize
				      nullptr                                     // *Arguments
				  );

    if(!size) {
        throw std::runtime_error("could not get error message "s + std::to_string(error_number));
    }

    // make sure buffer is freed
	auto message_ptr = std::unique_ptr<std::remove_pointer_t<LPSTR>,void(*)(LPSTR c)>(message_buffer, [](LPSTR c){ ::LocalFree(c); });

    LPSTR pos = std::find(message_buffer, message_buffer + size, '\r');
    std::string message(message_buffer, pos);

    if(message.empty()) {
        throw std::runtime_error("could not get error message (message empty) "s + std::to_string(error_number));
    }

    return message;
}

namespace _detail {
    // From windows to utf8 encoded std::string

    //! get size of utf8 encoded string
    /*!
     * Function that calculates the size of the string after conversion
     * @param[in]   in_string       LPCWSTR that will be converted to utf8 encoded std::string
     * @param[in]   in_string_size  size of in_string if it is not null-terminated
     * @return                      size of in_string if it would be converted to utf8 string
     */
    inline int string_from_win_get_size(wchar_t const* in_string, int const in_string_size = -1){
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            in_string_size,         // int     cchWideChar
            nullptr,                // LPSTR   lpMultiByteStr -> char* (OUTPUT)
            0,                      // int     cbMultiByte    -  set to 0 to get required size
            nullptr,                // LPCSTR  lpDefaultChar
            nullptr                 // LPBOOL  lpUsedDefaultChar
        );
    };

    //! convert LPWSTR and write result in provided buffer
    /*!
     * Functions that converts a given LPCWSTR into a utf8 encoded string
     * @param[in|out]   out_string          char* buffer that will contain the converted string
     *                                      it must have the size of out_string_size
     * @param[in]       out_string_size     int that describes the size of out out_string
     * @param[in]       in_string           LPCWSTR that will be converted to utf8 encoded std::string
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              number of written bytes or 0 on failure
     */
    inline int string_from_win(char* out_string, int out_string_size, wchar_t const* in_string, int const in_string_size=-1) {
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            in_string_size,         // int     cchWideChar
            out_string,             // LPSTR   lpMultiByteStr -> char*
            out_string_size,        // int     cbMultiByte
            nullptr,                // LPCSTR  lpDefaultChar
            nullptr                 // LPBOOL  lpUsedDefaultChar
        );
    };

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // From utf8 to windows encoded std::wstring

    //! get size of resulting LPWSTR string (utf16)
    /*!
     * Function that calculates the size of the resulting wstring after conversion
     * @param[in]   in_string       uft8 encoded const char* that will be converted to LPWSTR
     * @param[in]   in_string_size  size of in_string if it is not null-terminated
     * @return                      size of in_string if it would be converted to LPWSTR
     */
    inline int string_to_win_get_size(const char* in_string, int const in_string_size=-1) {
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            0,                      // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            nullptr,                // LPWSTR   lpWideCharStr
            0                       // int      cchWideChar - 0 to ask for required size
        );
    };

    //! convert char* to LPWSTR and write result in provided buffer
    /*!
     * Functions that converts a given utf8 encoded char* string into a LPCWSTR string
     * @param[in|out]   out_string          LPWSTR buffer that will contain the converted string
     *                                      it must have the size of out_string_size
     * @param[in]       out_string_size     int that describes the size of out out_string
     * @param[in]       in_string           utf8 encoded char* that will be converted to LPWSTR
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              number of written bytes or 0 on failure
     */
    inline int string_to_win(wchar_t* out_string, int out_string_size, const char* in_string, int const in_string_size=-1) {
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            0,                      // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            out_string,             // LPWSTR   lpWideCharStr
            out_string_size         // int      cchWideChar
        );
    };

} // _detail

    //! convert LPWSTR into utf8 encoded std::string
    /*!
     * Functions that converts a given LPCWSTR into an utf8 encoded std::string
     * @param[in]       in_string           LPCWSTR that will be converted to utf8 encoded std::string
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              utf8 encoded std::string
     */
    inline std::string string_from_win(wchar_t const* in_string, int const in_string_size = -1) {
        using namespace std::literals::string_literals;
        if (in_string_size == 0)  {
            return ""s;
        }

        int out_size = _detail::string_from_win_get_size(in_string, in_string_size);
        std::string rv(out_size, '$');
        int size = _detail::string_from_win(&rv[0], out_size, in_string, in_string_size);
        if (size == 0) {
            auto message = win_error_to_string(::GetLastError());
            throw std::runtime_error(message);
        }
        return rv;
    }

    inline std::string string_from_win(const std::wstring& in_string) {
        return string_from_win(in_string.c_str(), static_cast<int>(in_string.size()));
    }

    //! convert const char* into windows encoded std::wstring
    /*!
     * Functions that converts a given utf8 encoded char* into an std::wstring (data [LPWSTR])
     * @param[in]       in_string           utf8 encoded const char* string that will be converted to std::wstring
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              windows encoded encoded std::wstring
     */
    inline std::wstring string_to_win(const char* in_string, int const in_string_size = -1) {
        using namespace std::literals::string_literals;
        if (in_string_size == 0)  {
            return L""s;
        }

        int out_size = _detail::string_to_win_get_size(in_string, in_string_size);
        std::wstring rv(out_size, L'$');
        int size = _detail::string_to_win(&rv[0], out_size, in_string, in_string_size);
        if (size == 0) {
            auto message = win_error_to_string(::GetLastError());
            throw std::runtime_error(message);
        }
        return rv;
    }

    inline std::wstring string_to_win(const std::string& in_string) {
        return string_to_win(in_string.c_str(), static_cast<int>(in_string.size()));
    }
}}

#endif // EXT_WINDOWS
#endif // EXT_UTIL_WINDOWS_STRINGS_HEADER
