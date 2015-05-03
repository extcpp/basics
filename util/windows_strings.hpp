// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
//
// Collection of functions that convert between utf8 encoded
// strings (std::string char*) and strings taken by the windows
// API LPWSTR.
// For more information please refer to:
// - https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072.aspx (MultiByteToWideChar)
// - https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130.aspx (WideCharToMultiByte)
//
// Notes:
// the order out in is not lucky but instring_size is the only optional argument
// think about possible normalization
//
//
//              non UNICODE             UNICODE
//  LPCTSTR:    LPCSTR (const char*)    LPCWSTR (const wchar)
//  LPTSTR:     LPSTR (char*)           LPWSTR (wchar)
#pragma once

#ifdef _WIN32
#include <windows.h>
#include <string>

namespace obi { namespace util {


// form windows to utf8 encoded std::string

    //! get size of utf8 encoded string
    /*!
     * Function that calculates the size of the string after conversion
     * @param[in]   in_string       LPCWSTR that will be converted to utf8 encoded std::string
     * @param[in]   in_string_size  size of in_string if it is not null-terminated
     * @return                      size of in_string if it would be converted to utf8 string
     */
    int string_from_win_get_size(LPCWSTR in_string, int in_string_size=-1){
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            in_string_size,         // int     cchWideChar
            NULL,                   // LPSTR   lpMultiByteStr -> char* (OUTPUT)
            0,                      // int     cbMultiByte    -  set to 0 to get required size
            NULL,                   // LPCSTR  lpDefaultChar
            NULL                    // LPBOOL  lpUsedDefaultChar
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
    int string_from_win(char* out_string, int out_string_size, LPCWSTR in_string, int in_string_size=-1){
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            -1,                     // int     cchWideChar
            out_string,             // LPSTR   lpMultiByteStr -> char*
            out_string_size         // int     cbMultiByte
            NULL,                   // LPCSTR  lpDefaultChar
            NULL                    // LPBOOL  lpUsedDefaultChar
        );
    };

    //! convert LPWSTR into utf8 encoded std::string
    /*!
     * Functions that converts a given LPCWSTR into an utf8 encoded std::string
     * @param[in]       in_string           LPCWSTR that will be converted to utf8 encoded std::string
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              utf8 encoded std::string
     */
    std::string string_from_win(LPCWSTR in_string, int in_string_size=-1){
        int size = string_from_win_get_size(in_string, in_string_size);
        std::string rv(size, '');
        string_from_win(&rv[0], size, in_string, instring_size);
        return rv;
    }

    // overload for std::wstring
    std::string string_from_win(const std::wstring& in_string){
        return string_from_win(in_string.c_str());
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //! get size of resulting LPWSTR string (utf16)
    /*!
     * Function that calculates the size of the resulting wstring after conversion
     * @param[in]   in_string       uft8 encoded const char* that will be converted to LPWSTR
     * @param[in]   in_string_size  size of in_string if it is not null-terminated
     * @return                      size of in_string if it would be converted to LPWSTR
     */
    int string_to_win_get_size(const char* in_string, int in_string_size=-1){
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            MB_PRECOMPOSED,         // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            NULL,                   // LPWSTR   lpWideCharStr
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
    int string_to_win(LPWSTR out_string, int out_string_size, const char* in_string, int in_string_size=-1){
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            MB_PRECOMPOSED,         // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            out_string,             // LPWSTR   lpWideCharStr
            out_string_size         // int      cchWideChar
        );
    };

    //! convert const char* into windows encoded std::wstring
    /*!
     * Functions that converts a given utf8 encoded char* into an std::wstring (data [LPWSTR])
     * @param[in]       in_string           utf8 encoded const char* string that will be converted to std::wstring
     * @param[in]       in_string_size      size of in_string if it is not null-terminated
     * @return                              windows encoded encoded std::wstring
     */
    std::wstring string_to_win(const char* in_string, int in_string_size=-1){
        int size = string_to_win_get_size(in_string, in_string_size);
        std::wstring rv(size, L'');
        string_to_win(&rv[0], size, in_string, in_string_size);
        return rv;
    }

    // overload for std::string
    std::wstring string_to_win(const std::string& in_string){
        return string_to_win(in_string.c_str());
    }
}}
#endif
