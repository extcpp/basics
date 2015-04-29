#pragma once

#ifdef _WIN32
#include <windows.h>
#include <string>

// I define char* utf-8 encoded to be the default
// the order out in is not lucky but instring_size is the only optional argument

namespace obi { namespace util {

//              non UNICODE             UNICODE
//  LPCTSTR:    LPCSTR (const char*)    LPCWSTR (const wchar)
//  LPTSTR:     LPSTR (char*)           LPWSTR (wchar)


// form windows to utf8 encoded std::string
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

    std::string string_from_win(LPCWSTR in_string, int in_string_size=-1){
        int size = string_from_win_get_size(in_string, in_string_size);
        std::string rv;
        rv.resize(size);
        string_from_win(&rv[0], size, in_string, instring_size);
        return rv;
    }

    std::string string_from_win(const std::wstring in_string){
        return string_from_win(in_string.c_str());
    }



// form utf8 encoded std::string to windows
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

    std::wstring string_to_win(const char* in_string, int in_string_size=-1){
        int size = string_to_win_get_size(in_string, in_string_size);
        std::wstring rv;
        rv.resize(size);
        string_to_win(&rv[0], size, in_string, in_string_size);
        return rv;
    }

    std::wstring string_to_win(const char* in_string, int in_string_size=-1){
        return string_to_win(in_string.c_str());
    }
}}
#endif
